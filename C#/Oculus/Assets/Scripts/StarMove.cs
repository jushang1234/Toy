using UnityEngine;
using System.Collections;

public class StarMove : MonoBehaviour {

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {
		transform.Translate (0, 0, -Time.deltaTime*50);

		if (transform.position.z < -50) {
			transform.position = StarSpawn.GenStarPosition();
		}
		if (transform.position.x * transform.position.x + transform.position.y * transform.position.y < 25) {			
			transform.position = StarSpawn.GenStarPosition();
		}
	}
}
