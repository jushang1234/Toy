using UnityEngine;
using System.Collections;

public class FollowAvartar : MonoBehaviour {

	public GameObject avartarAtom;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.position = new Vector3 (avartarAtom.transform.position.x, avartarAtom.transform.position.y, transform.position.z);
	}
}
