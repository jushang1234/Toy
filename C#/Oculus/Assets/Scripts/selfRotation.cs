﻿using UnityEngine;
using System.Collections;

public class SelfRotation : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.Rotate(new Vector3(-Time.deltaTime,0,0));
	}
}
