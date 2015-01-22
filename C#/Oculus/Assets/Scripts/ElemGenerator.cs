using UnityEngine;
using System.Collections;

public class ElemGenerator : MonoBehaviour {

	public Object m_ElementPrefab;
	public float m_GenTime;
	public float[] m_GenRangeX = { 3f, 10f };
	public float[] m_GenRangeY = { 2f, 5f };
	public ArrayList m_ElemList = new ArrayList();
	private float m_genTime;

	// Use this for initialization
	void Start () {
		StartCoroutine(HitGeneratePoint(1.0f));
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void GenerateElem(Object prefab) {
		float x = Random.Range( m_GenRangeX[0], m_GenRangeX[1] );
		float y = Random.Range( m_GenRangeY[0], m_GenRangeY[1] );
		x = Random.Range(0,2) >= 1 ? -x : x;
		y = Random.Range(0,2) >= 1 ? -y : y;
		Vector3 pos = new Vector3( x, y, 50f );
		GameObject elemObj = Instantiate( prefab, pos, Quaternion.identity ) as GameObject;
		Element elem = elemObj.GetComponent<Element>();
		m_ElemList.Add( elem );
		elem.m_Mgr = this;
	}

	public void OnElemDisappear( Element elem ) {
		m_ElemList.Remove( elem );
	}

	IEnumerator HitGeneratePoint( float time ) {
		m_genTime = time;
		while (m_genTime > 0) {
			if (GameMgr.State != GameState.PAUSE) {
				m_genTime -= Time.deltaTime;
			}
			yield return new WaitForFixedUpdate();
		}
		GenerateElem(m_ElementPrefab);
		StartCoroutine(HitGeneratePoint(m_GenTime));
	}
}
