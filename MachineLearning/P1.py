__author__ = 'shangju'
# % maint:  1st column  vhigh = 1, high = 2, med = 3, low = 4.
# %
# % safety:  2nd column  low = 1, med = 2, high = 3.
# % choose label either <=0 or >0
# % build the probability table p(maint|safety,label>0), p(maint|safety,label<=0)
# % p(safety|maint,label>0), p(safety|maint,label<=0

import numpy as np

def filetomatrix(filename):
    fr = open(filename)
    arrayoflines = fr.readlines()
    output = []
    for line in arrayoflines:
        line = line.strip('\n')
        listfromline = line.split("	")
        listfromline = [int(item) for item in listfromline]
        output.append(listfromline)
    return output

def maintprob(input):
    matrix1 = np.zeros((3,4))
    matrix2 = np.zeros((3,4))
    for safty in range(1,4):
        base = 0
        base2 = 0
        i = 0
        j = 0
        k = 0
        l = 0
        a = 0
        b = 0
        c = 0
        d = 0
        for data in input:
            if data[2]>0:
                if data[1] == safty:
                    base += 1
                    if data[0] == 1:
                        i+=1
                    elif data[0] == 2:
                        j+=1
                    elif data[0] == 3:
                        k+=1
                    elif data[0] == 4:
                        l+=1
            else:
                if data[1] == safty:
                    base2 += 1
                    if data[0] == 1:
                        a+=1
                    elif data[0] == 2:
                        b+=1
                    elif data[0] == 3:
                        c+=1
                    elif data[0] == 4:
                        d+=1
        main = []
        main.append(i)
        main.append(j)
        main.append(k)
        main.append(l)
        main2 = []
        main2.append(a)
        main2.append(b)
        main2.append(c)
        main2.append(d)
        p = -1
        for result in main:
            p=p+1
            if base == 0:
                matrix1[safty-1][p]=-1
                # print"p(maint=",  p,"|safety=",(safty),",label>0)= 0  or infinity"
            else:
                haha =  float(result)/float(base)
                matrix1[safty-1][p]=haha
                # print"p(maint=",  p,"|safety=",(safty),",label>0)=",haha
        p = -1
        for result in main2:
            p=p+1
            if base2 == 0:
                matrix2[safty-1][p]=-1
                # print"p(maint=",  p,"|safety=",(safty),",label<=0)= 0  or infinity"
            else:
                haha =  float(result)/float(base2)
                matrix2[safty-1][p]=haha
                # print"p(maint=",  p,"|safety=",(safty),",label<=0)=",haha
    print("safty:maint, where label>0:\n")
    print(matrix1)
    print "\n"
    print("safty:maint, where label<=0:\n")
    print(matrix2)
    print "\n"



def saftyprob(input):
    matrix3 = np.zeros((4,3))
    matrix4 = np.zeros((4,3))
    for maint in range(1,5):
        base = 0
        base2 = 0
        i = 0
        j = 0
        k = 0
        a = 0
        b = 0
        c = 0
        for data in input:
            if data[2]>0:
                if data[0] == maint:
                    base += 1
                    if data[1] == 1:
                        i+=1
                    elif data[1] == 2:
                        j+=1
                    elif data[1] == 3:
                        k+=1
            else:
                if data[0] == maint:
                    base2 += 1
                    if data[1] == 1:
                        a+=1
                    elif data[1] == 2:
                        b+=1
                    elif data[1] == 3:
                        c+=1
        main = []
        main.append(i)
        main.append(j)
        main.append(k)
        main2 = []
        main2.append(a)
        main2.append(b)
        main2.append(c)
        p = -1
        for result in main:
            p=p+1
            if base == 0:
                matrix3[maint-1][p]=-1
                # print"p(safety= ",  p,"|maint= ",(maint),",label>0)= 0 or infinity"
            else:
                haha =  float(result)/float(base)
                matrix3[maint-1][p]=haha
                # print"p(safety= ",  p,"|maint= ",(maint),",label>0)=",haha
        p = -1
        for result in main2:
            p=p+1
            if base2 == 0:
                matrix4[maint-1][p]=-1
                # print"p(safety= ",  p,"|maint= ",(maint),",label<=0)= 0 or infinity"
            else:
                haha =  float(result)/float(base2)
                matrix4[maint-1][p]=haha
                # print"p(safety= ",  p,"|maint= ",(maint),",label<=0)=",haha
    print("maint:safety, where label>0:\n")
    print(matrix3)
    print("\n")
    print("maint:safety, where label<=0:\n")
    print(matrix4)
    print("\n")

if __name__ == '__main__':
    result = filetomatrix('CarData')
    print("\n")
    print("results:\n")
    print("(\'-1\' denote NULL entries)\n\n")
    saftyprob(result)
    maintprob(result)
    # for items in result:
    #     if items[2]>0:
    #         if items[0] ==1:
    #             print items
