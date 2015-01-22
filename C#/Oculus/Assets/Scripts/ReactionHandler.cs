using System;
using UnityEngine;
using System.Collections.Generic;

public class ReactionHandler: MonoBehaviour{

	public ReactionTable m_ReactionTable;
	public AudioSource m_ReactSound;

	private float m_timer = 0f;
	private ReactionTable.ReactionRule m_currentRule;
	private AvatarCtrl m_control;

	public void CheckReaction(AvatarCtrl control){
		m_control = control;
		m_currentRule = m_ReactionTable.CanReact (control.m_Elements);
		if(m_currentRule != null){
			foreach(int i in m_ReactionTable.GetConsumed(m_control.m_Elements, m_currentRule)){
				control.m_Elements[i].GetComponent<Element>().m_IsConsumed = true;
			}
			m_ReactSound.Play();
			m_currentRule.StartParticles(control.m_Aimer.transform);
		}
	}

	public void Update(){
		if(m_currentRule == null)return;

		m_timer += Time.deltaTime;
		if (m_timer >= 1.5f) {
			m_timer = 0f;
			React ();
		}
	}

	public void React(){
		m_currentRule.StopParticles ();
		foreach(int i in m_ReactionTable.GetConsumed(m_control.m_Elements, m_currentRule)){
			Element e = m_control.PopElement(i);
			Destroy(e.gameObject);
		}

		foreach(String name in m_currentRule.Products()){
			GameObject obj = (GameObject)Instantiate(m_ReactionTable.GetSpawned(name), m_control.m_Aimer.position, Quaternion.identity);
			m_control.GrabElement(obj.GetComponent<Element>());
		}

		m_currentRule = null;
	}
}