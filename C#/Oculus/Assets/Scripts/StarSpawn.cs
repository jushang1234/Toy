using UnityEngine;
using System.Collections;

public class StarSpawn : MonoBehaviour {

	public GameObject starPrefab;

	// Use this for initialization
	void Start () {
		for (int i= 1; i<100; i++) {
			GameObject starObj = Instantiate(starPrefab,GenStarPosition(),Quaternion.identity) as GameObject;
			starObj.transform.parent = transform;
		}
	
	}

	public static Vector3 GenStarPosition(){
		return new Vector3 (Random.Range (-20, 20), Random.Range (-20, 20), Random.Range (-50, 50));
	}
}
