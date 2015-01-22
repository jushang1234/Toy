using UnityEngine;
using System.Collections;

public class Element : MonoBehaviour {
	public enum State {
		IDLE = 1, AIMED = 2, ATTACHED = 3, REACTION = 4
	};
	
	public static float		m_DashDistance = 0f;

	public bool				m_IsObstacle;
	public float[] 			m_Speed = new float[]{2f,4f};
	public float[]			m_RotationSpeedRange = new float[]{20f,40f};
	public float 			m_DisappearZ = -50f;
	public ElemGenerator 	m_Mgr;
	public bool				m_IsConsumed;
	public State 			m_State = Element.State.IDLE;
	public AvatarCtrl 		m_Avatar;
	public Transform	 	m_Sphere;
	public Transform[] 		m_Center = new Transform[1];
	public GameObject		m_OVRCamRig;
	private float[] 		m_rotateSpeed;
	private float 			m_speed;
	private Vector3 		m_baseColliderScale;
	private float 			m_originalZ;
	private GameObject 		m_squareObj;
	private Vector3 		m_squareShrinkSpeed;
	private Vector3 		m_oriSquareScale;
	private int		 		m_squareShrinkFlag = -1;
	private float 			m_aimTime = 0f;


	// Use this for initialization
	void Start () {
		SetState( State.IDLE );
		m_rotateSpeed = new float[m_Center.Length];
		for( int i=0; i<m_Center.Length; i++) {
			float range = m_RotationSpeedRange[1] - m_RotationSpeedRange[0];
			m_rotateSpeed[i] = Random.value*range + m_RotationSpeedRange[0];
			int flag = Random.Range( 0, 2 ) >= 1 ? 1 : -1;
			m_rotateSpeed[i] *= flag;
		}
		m_speed = Random.Range( m_Speed[0], m_Speed[1] );
		float factor = 0.2f;
		m_baseColliderScale = m_Sphere.localScale;
		m_originalZ = transform.position.z;
		float speed = 0.2f;
		m_squareShrinkSpeed = new Vector3(speed,speed,speed);
	}
	
	// Update is called once per frame
	void Update () {
		ProcessCenterRotate();
		ProcessMove();
		ProcessAttachAnim();
		ProcessCheckAttach();
		ProcessCheckReaction();
		if( transform.position.z < m_DisappearZ ) {
			if(m_Mgr) {
				m_Mgr.OnElemDisappear(this);
			}
			Destroy(gameObject);
		}
	}

	void FixedUpdate() {
		ProcessCheckUnAimed();
	}

	public void SetState( State state ) {
		m_State = state;
	}

	public void OnAimed( AvatarCtrl avatar ) {
		SetState( State.AIMED );
		m_Avatar = avatar;
		m_squareObj = Instantiate( avatar.m_AimedPrefab, transform.position, Quaternion.Euler(270f, 0, 0) ) as GameObject;
		m_squareObj.transform.parent = transform;
		m_aimTime = 0f;
		m_Avatar.m_Beep.Play ();
	}

	public void UnAimed() {
		Destroy(m_squareObj);
		m_squareObj = null;
		m_Avatar.OnUnAimed();
		SetState( State.IDLE );
	}

	void Attached() {
		SetState( State.ATTACHED );
		m_oriSquareScale = m_squareObj.transform.localScale;
		m_squareShrinkFlag = -1;
		m_aimTime = 0f;
	}

	void Reacted(){
		SetState (State.REACTION);
		m_aimTime = 0f;
		m_Avatar.m_Thrust.Play ();
		m_Avatar.m_IsCameraShocking = true;
		m_DashDistance = transform.position.z - m_Avatar.transform.position.z;
	}

	void ProcessCheckUnAimed() {
		if( m_State != State.AIMED && m_State != State.ATTACHED ) return;
		Ray ray = m_Avatar.m_Camera.ScreenPointToRay ( m_Avatar.m_Camera.WorldToScreenPoint(m_Avatar.m_Aimer.position) );
		RaycastHit[] hits = Physics.RaycastAll( ray.origin, ray.direction );
		foreach( RaycastHit hit in hits ) {
			if( !hit.transform || !hit.transform.gameObject.activeSelf ) continue;
			if(!hit.transform.gameObject.CompareTag("Aimable")) continue;
			Element elem = hit.transform.parent.gameObject.GetComponent<Element>();
			if( !elem ) continue;
			if( elem == this ) return;
		}
		UnAimed();
	}

	void ProcessCheckAttach() {
		if( m_State != State.AIMED ) return;
//		if( transform.position.z - m_Avatar.transform.position.z > 4f ) return;
		m_aimTime += Time.deltaTime;
		if(m_aimTime < 1.0f) return;
		Attached();
	}

	void ProcessAttachAnim() {
		if( m_State != State.ATTACHED ) return;
		m_squareObj.transform.localScale += m_squareShrinkFlag * m_squareShrinkSpeed * Time.deltaTime;
		if(m_squareShrinkFlag < 0) {
			if(m_squareObj.transform.localScale.x < m_oriSquareScale.x*0.8f) {
				m_squareShrinkFlag = -m_squareShrinkFlag;
			}
		} else {
			if(m_squareObj.transform.localScale.x > m_oriSquareScale.x) {
				m_squareShrinkFlag = -m_squareShrinkFlag;
			}
		}
	}

	void ProcessCheckReaction() {
		if( m_State != State.ATTACHED ) return;
		m_aimTime += Time.deltaTime;
		if(m_aimTime < 0.8f) return;
		Reacted ();
	}

	void ProcessCenterRotate() {
		for( int i=0; i<m_Center.Length; i++ ) {
			m_Center[i].Rotate (0, 0, m_rotateSpeed[i]*Time.deltaTime);
		}
	}

	void ProcessMove() {
		if (m_State != State.REACTION) {
			transform.Translate (Vector3.back * (m_speed + m_DashDistance * 5f) * Time.deltaTime);
		}else //if(m_DashDistance > 0.1f){
			transform.position = Vector3.Lerp(transform.position, m_Avatar.transform.position+Vector3.back, 5f * Time.deltaTime);
		//}else
		//	UnAimed();

		float scale = 6f * (transform.position.z - m_DisappearZ) / (m_originalZ - m_DisappearZ) - 2f;
		if (!m_IsObstacle)
			m_Sphere.localScale = m_baseColliderScale * Mathf.Max(1f, scale);
	}

	//unused
	void OnCollisionEnter(Collision collision){
		/*
		Debug.Log ("Kno' kno' knocking on element");
		Transform sphere = collision.gameObject.transform;
		if(!sphere.CompareTag("Captured")) return;
		*/
		//Debug.Log ("Kno' kno' knocking on captured element");
	}
}
