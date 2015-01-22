using UnityEngine;
using System.Collections;

public class AvatarCtrl : MonoBehaviour {

	public GameMgr 			m_Mgr;
	public ElemGenerator 	m_ElemGen;
	public Transform[] 		m_Elements;
	public int				m_ElementSize;
	public float			m_ElementSpreadRadius;
	public Transform 		m_CenterEye;
	public Transform 		m_Aimer;
	public GameObject 		m_AimedPrefab;
	public GameObject 		m_spaceShip;
	public Camera 			m_MainCamera;
	public Camera 			m_OculusCamera;
	public Camera 			m_Camera;
	public AudioSource		m_Beep;
	public AudioSource		m_Thrust;
	public AudioSource 		m_captureSound;
	public GameObject 		m_OVRCamRig;
	public bool				m_IsCameraShocking;
	private Transform 		m_eye;
	private float[] 		m_lastEyeAngle = {0,0};
	private float 			m_smooth = 5f;
	private Vector3 		m_moveSpeed;
	private Element			m_aimedElem;
	
	// Use this for initialization
	void Start () {
//		Application.targetFrameRate = 60;
//		print (Application.targetFrameRate);
		m_eye = m_Mgr.m_IsUseOculus ? m_CenterEye : m_MainCamera.transform;
		m_Camera = m_Mgr.m_IsUseOculus ? m_OculusCamera : m_MainCamera;
		GrabElement (m_Elements [0].gameObject.GetComponent<Element>());
	}
	
	// Update is called once per frame
	void Update () {
		ProcessOxygenRotate ();
		ProcessAvatarMove ();
		ProcessInput();
		ProcessDash ();
		ProcessCameraShock ();
	}

	void FixedUpdate() {
		ProcessCheckTarget();
	}

	void AimElem( Element elem ) {
		m_aimedElem = elem;
		elem.OnAimed(this);
	}

	public void OnUnAimed() {
		m_aimedElem = null;
		m_Beep.Stop ();
	}

	public bool GrabElement(Element elem){
		if (m_ElementSize >= m_Elements.Length)	PopElement (0);
		m_Elements [m_ElementSize++] = elem.transform;
		elem.gameObject.GetComponent<Element> ().enabled = false;
		elem.gameObject.transform.FindChild ("Sphere").gameObject.tag = "Captured";
		m_captureSound.Play ();
		elem.transform.SetParent (transform);

		gameObject.GetComponent<ReactionHandler> ().CheckReaction (this);
		return true;
	}

	public Element PopElement(int index){
		Element head = m_Elements [index].gameObject.GetComponent<Element>();
		for (int i = index+1; i < m_ElementSize; i ++) {
			m_Elements[i-1] = m_Elements[i];
		}
		m_Elements [--m_ElementSize] = null;
		head.enabled = true;
		head.gameObject.transform.FindChild ("Sphere").gameObject.tag = "Aimable";
		head.transform.SetParent (transform.root);

		return head;
	}

	void ProcessDash(){
		if (Mathf.Abs(Element.m_DashDistance) > .1f)
			Element.m_DashDistance *= (1-5f * Time.deltaTime);
		else if(m_aimedElem != null && m_aimedElem.m_State == Element.State.REACTION){
			if(m_aimedElem.m_IsObstacle){ // uh oh
				for(int i=m_ElementSize;i>0;i--)PopElement(0);
			}else{
				GrabElement(m_aimedElem);
			}
			m_aimedElem.UnAimed();
		}
	}

	void ProcessCheckTarget() {
		if( m_aimedElem ) return;
		Ray ray = m_Camera.ScreenPointToRay ( m_Camera.WorldToScreenPoint(m_Aimer.position) );
		RaycastHit[] hits = Physics.RaycastAll( ray.origin, ray.direction );
		foreach( RaycastHit hit in hits ) {
			if( !hit.transform || !hit.transform.gameObject.activeSelf ) continue;
			if(!hit.transform.gameObject.CompareTag("Aimable")) continue;
			Element elem = hit.transform.parent.gameObject.GetComponent<Element>();
			if( !elem ) continue;
			AimElem( elem );
			break;
		}
	}

	void ProcessOxygenRotate() {
		float toAngle = 50f * Time.time * Mathf.PI / 180f;
		float spread = Mathf.PI * 2 / m_ElementSize;
		for (int i = 0; i < m_ElementSize; i++) {
			Vector3 target = m_Aimer.position;
			if (!m_Elements[i].GetComponent<Element>().m_IsConsumed){
				target += new Vector3(
					Mathf.Cos (toAngle) * m_ElementSpreadRadius,
					Mathf.Sin (toAngle) * m_ElementSpreadRadius,
					0f);
			}				

			m_Elements[i].transform.position = Vector3.Lerp(m_Elements[i].transform.position, target, m_smooth * .5f * Time.deltaTime);
			toAngle += spread;
		}
		//m_Oxygen.Rotate (0, angle, 0);
	}

	void ProcessAvatarMove() {
		float[] eyeAngle = { m_eye.rotation.eulerAngles.y, -m_eye.rotation.eulerAngles.x };
		Vector3 newPos = new Vector3(1,1,1);
		bool isChanged = false;
		for( int i=0; i<eyeAngle.Length; i++ ) { // i=0:x, i=1:y
			newPos[i] = Mathf.Sin(eyeAngle[i]*Mathf.PI/180f);
			newPos[2]*= Mathf.Cos(eyeAngle[i]*Mathf.PI/180f);
			if( Mathf.Abs( eyeAngle[i]-m_lastEyeAngle[i] ) < 0.5f ) {
				continue;
			} 
			m_lastEyeAngle[i] = eyeAngle[i];
			isChanged = true;
//			newPos[2] *= Mathf.Cos(eyeAngle[i]*Mathf.PI/180f);
//			float pos = eyePos[i] / (2*Cfg.EyeBoundary) * (2*Cfg.Boundary[i*2+1]) - Cfg.Boundary[i*2+1];
//			if(i==0) m_tarPos.x = pos;
//			else m_tarPos.y = pos;
		}

		m_Aimer.position = new Vector3(newPos[0] * Cfg.EyeBoundary, 
		                               newPos[1] * Cfg.EyeBoundary, 
		                               newPos[2] * Cfg.EyeBoundary * 0.6f + m_Camera.transform.position.z);

		LerpTo (this.transform, m_Aimer);
						
	}

	void LerpTo(Transform from, Transform to){
		float dist = Vector3.Distance(from.position, to.position);
		if (dist <= 0.1f)return;
		from.position = Vector3.Lerp (from.position, to.position, m_smooth * Time.deltaTime);
	}

	void ProcessInput() {
		float speed = 20f;
		if( Input.GetKey( KeyCode.W ) ) {
			m_MainCamera.transform.Rotate( -speed*Time.deltaTime, 0, 0 );
		}else if( Input.GetKey( KeyCode.S ) ) {
			m_MainCamera.transform.Rotate( speed*Time.deltaTime, 0, 0 );
		}
		if( Input.GetKey( KeyCode.A ) ) {
			m_MainCamera.transform.Rotate( 0, -speed*Time.deltaTime, 0 );
		}else if( Input.GetKey( KeyCode.D ) ) {
			m_MainCamera.transform.Rotate( 0, speed*Time.deltaTime, 0 );	
		}

	}

	void ProcessCameraShock () {
		if (m_IsCameraShocking) {
			if (m_OVRCamRig.transform.position.z <  -4.1f){
				m_OVRCamRig.transform.position = Vector3.Lerp (m_OVRCamRig.transform.position, new Vector3 (0, 0, -4), 20f*Time.deltaTime);
				m_spaceShip.transform.position = Vector3.Lerp (m_spaceShip.transform.position, new Vector3 (0, 0, -3), 20f*Time.deltaTime);
			}
			else {
			m_IsCameraShocking = false;
			}
		}
		if (!m_IsCameraShocking && m_OVRCamRig.transform.position != new Vector3 (0, 0, -8)) {
			m_OVRCamRig.transform.position = Vector3.Lerp (m_OVRCamRig.transform.position, new Vector3 (0, 0, -8), 3f*Time.deltaTime);
			m_spaceShip.transform.position = Vector3.Lerp (m_spaceShip.transform.position, new Vector3 (0, 0, -8.275f), 3f*Time.deltaTime);
		}
	}
}
