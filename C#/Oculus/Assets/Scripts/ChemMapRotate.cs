using UnityEngine;
using System.Collections;

public class ChemMapRotate : MonoBehaviour {

	public GameObject centAnch;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (centAnch.transform.rotation.eulerAngles.x >= 10 && centAnch.transform.rotation.eulerAngles.x <= 180 ){
			transform.RotateAround(Vector3.zero,Vector3.right,(10 - centAnch.transform.rotation.eulerAngles.x)*Time.deltaTime*2f);
		}
		
		else if (centAnch.transform.rotation.eulerAngles.x >= 180 && centAnch.transform.rotation.eulerAngles.x <= 350 ){
			transform.RotateAround(Vector3.zero,Vector3.right,(350 - centAnch.transform.rotation.eulerAngles.x)*Time.deltaTime*2f);
		}

		if (centAnch.transform.rotation.eulerAngles.y >= 30 && centAnch.transform.rotation.eulerAngles.y <= 180 ){
			transform.RotateAround(Vector3.zero,Vector3.up,(30 - centAnch.transform.rotation.eulerAngles.y)*Time.deltaTime*2f);
		}

		else if (centAnch.transform.rotation.eulerAngles.y >= 180 && centAnch.transform.rotation.eulerAngles.y <= 330 ){
			transform.RotateAround(Vector3.zero,Vector3.up,(330 - centAnch.transform.rotation.eulerAngles.y)*Time.deltaTime*2f);
		}
	}
}
