__author__ = 'shangju'

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches



def filetomatrix(filename): # read the data to array
    fr = open(filename)
    arrayoflines = fr.readlines()
    output = []
    for line in arrayoflines:
        line = line.strip('\n')
        listfromline = line.split("	")
        floats = listfromline[0].strip(" ").split("   ")
        listfromline = [float(item) for item in floats]
        output.append(listfromline)
    return output

data = filetomatrix('CircleData')

def innercircle(input, center, flag):   # use iteration to find inner circle
    if flag == 0:                       # the flag is due to the change on train/test partition
        r = 0                           # intialize the radius as 0
        i = 0
        for items in input:
            if i < 15:
                while (items[0] - center[0]) * (items[0] - center[0]) + (items[1] - center[1]) * (
                            items[1] - center[1]) > r * r:
                    r = r + 0.5
            i += 1
        return [center, r]
    else:
        r = 0
        for items in input:
            while (items[0] - center[0]) * (items[0] - center[0]) + (items[1] - center[1]) * (
                        items[1] - center[1]) > r * r:
                r = r + 0.5
        return [center, r]



def outercircle(input, center, flag):       # use iteration to find outer circle
    if flag ==0:                            # flag 1 means the input are all neg, otherwise means all pos
        r = 500                             # intialize the radius as a relatively big number
        i = 0
        for items in input:
            if i >= 15:
                while (items[0] - center[0]) * (items[0] - center[0]) + (items[1] - center[1]) * (
                            items[1] - center[1]) < r * r:
                    r -= 1
            i += 1
        return [center, r]
    else:
        r = 500
        for items in input:
            while (items[0] - center[0]) * (items[0] - center[0]) + (items[1] - center[1]) * (
                        items[1] - center[1]) < r * r:
                r -= 1
        return [center, r]

def noerror(result):                        # iteration to find circle
    final1 = [185, 185]
    final2 = [185, 185]
    temp1 = 10000.0;
    temp2 = 0.00
    for i in range(150, 300):               # brutal force to find the center that maximize the S and G circle
        for j in range(150, 300):           # could implement A* algorithm later
            center = [i, j]
            innercircle1 = innercircle(result, center, 0)
            outercircle1 = outercircle(result, center, 0)
            if innercircle1[1] < temp1:
                temp1 = innercircle1[1]
                final1 = center
            if outercircle1[1] > temp2:
                temp2 = outercircle1[1]
                final2 = center
    innercirclefinal = innercircle(result, final1, 0)
    outercirclefinal = outercircle(result, final2, 0)

    return [innercirclefinal, outercirclefinal]

def plot(innercirclefinal, outercirclefinal, data):     # plotting
    ax = plt.subplot(111, aspect=1)
    i = 0
    for items in data:                                  # plot data point
        if i < 15:
            plt.scatter(items[0], items[1], c="b")
        else:
            plt.scatter(items[0], items[1], c="r")
        i += 1
    c = mpatches.Circle((innercirclefinal[0][0], innercirclefinal[0][1]), innercirclefinal[1], fill=False)      # plot first circle
    c2 = mpatches.Circle((outercirclefinal[0][0], outercirclefinal[0][1]), outercirclefinal[1], fill=False)     # plot second circle
    ax.add_patch(c)
    ax.add_patch(c2)
    plt.show()


def test(x,y, model):                           # return true/1 if the point is in the circle, return false/0 otherwise
    if (x-model[0][0])*(x-model[0][0])+(y-model[0][1])*(y-model[0][1]) < model[1]*model[1]:
        return 1
    else:
        return 0


def train(positive, negayive, s_center, g_center):  # find the h class with largest margin, I decide to use the half wa
    s_cir= innercircle(positive,s_center,1)
    g_cir= outercircle(negayive,g_center,1)
    h_x = (s_cir[0][0]+g_cir[0][0])/2
    h_y = (s_cir[0][1]+g_cir[0][1])/2
    h_r = (s_cir[1]+g_cir[1])/2
    h_cir = [[h_x,h_y],h_r]
    return h_cir


def demo():
    train_pos = []
    train_neg = []
    test_pos = []
    test_neg = []
    i = 0
    for items in data:
        if i < 10:
            i += 1
            train_pos.append(items)
        elif i < 15:
            i += 1
            test_pos.append(items)
        elif i < 25:
            i += 1
            train_neg.append(items)
        elif i < 30:
            i += 1
            test_neg.append(items)
    S_center = [176, 202]
    G_center = [197, 204]
    trained_data = train(train_pos,train_neg,S_center,G_center)
    print 'The h class we use (also is the margin circle) is: ', trained_data
    pos_errorcount = 0
    neg_errorcount = 0
    print '\nPredicting test points: \n'
    print "---TESTING POS POINT---\n"
    for items in test_pos:
        if (test(items[0],items[1],trained_data)):
            print items, "  correct"
            continue
        else:
            print items, "  error"
            pos_errorcount+=1
    print '\n---TESTING NEG POINTS---\n'
    for items in test_neg:
        if (test(items[0],items[1],trained_data)):
            neg_errorcount+=1
            print items, "   error"
        else:
            print items, "  correct"
            continue
    print '\n---END---\n'
    print 'error count for pos_test: ', pos_errorcount
    print 'error count for neg_test: ', neg_errorcount
    print '\nempirical error is ', pos_errorcount+neg_errorcount


if __name__ == '__main__':
    print '\n---Demo---\n'
    demo()
    print"\n---wait for maximum specific and general circle---\n"
    finalcir = noerror(data)
    print "\nThe most spercific and most general circles with its radius are:\n", finalcir
    plot(finalcir[0],finalcir[1],data)