using UnityEngine;
using System.Collections;

public class KeyboardHandler : MonoBehaviour {

	public AudioSource windShieldSound;
	public AudioSource teleportSound;
	public AudioSource bgm;
	public GameObject curtain;
	public GameObject windShield;

	Color transparent;
	public Color blackout;
	bool windShieldDown = false;
	bool windShieldUp = true;
	bool isBlackOut = false;
	// Use this for initialization
	void Start () {
		blackout.a = 0;
		curtain.renderer.material.color = blackout;
	
	}
	
	// Update is called once per frame
	void Update () {
	if (Input.GetKeyUp (KeyCode.Space)) {
			OVRManager.capiHmd.RecenterPose();
				}
	if (Input.GetKeyUp (KeyCode.Alpha1)) {
			ProcessBlackout();
		}

		ProcessWindShield();
		ProcessCurtain ();
	}

	public void ProcessBlackout(){
		if(isBlackOut)return;
		windShieldSound.Play();
		bgm.Stop();
		teleportSound.PlayDelayed(3.5f);
		windShieldDown =!windShieldDown;
		windShieldUp =!windShieldUp;
		isBlackOut = true;
	}

	void ProcessWindShield() {
		if (windShieldDown) {
			windShield.transform.position = Vector3.Lerp(windShield.transform.position,
			                                             new Vector3(windShield.transform.position.x,0.15f,windShield.transform.position.z),
			                                             Time.deltaTime);
		}

		if (windShieldUp) {
			windShield.transform.position = Vector3.Lerp(windShield.transform.position,
			                                             new Vector3(windShield.transform.position.x,2.25f,windShield.transform.position.z),
			                                             Time.deltaTime);
		}
		}

	void ProcessCurtain() {
		if (isBlackOut && blackout.a <= 1f) {
						blackout.a += 0.12f*Time.deltaTime;
						curtain.renderer.material.color = blackout;
				} 

		if (blackout.a >= 0.99f) {
				Application.LoadLevel("main");
				}
				}
}