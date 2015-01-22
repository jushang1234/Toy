using UnityEngine;
using System.Collections;

public class SpaceShipCtrl : MonoBehaviour {

	public GameObject OVRCamRig;

	
	private float lastCamAngleX = 0f;
	private float lastCamAngleZ = 0f;
	private int   timeCounter = 0;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {

		transform.Rotate(OVRCamRig.transform.rotation.eulerAngles.x-lastCamAngleX,0,-OVRCamRig.transform.rotation.eulerAngles.y+lastCamAngleZ);
		lastCamAngleX = OVRCamRig.transform.rotation.eulerAngles.x;
		lastCamAngleZ = OVRCamRig.transform.rotation.eulerAngles.y;

		//transform.Translate (Mathf.Sin (timeCounter/70)/400, Mathf.Cos (timeCounter/50)/200, Mathf.Sin (timeCounter/30)/200);
		//timeCounter ++;

	}
}
