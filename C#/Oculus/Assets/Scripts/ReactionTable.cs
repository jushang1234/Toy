using System;
using System.Collections.Generic;
using UnityEngine;
using System.Collections;

[Serializable]
public class ReactionTable
{
	[Serializable]
	public class ElementLookup{
		public String name;
		public GameObject prefab;
	}

	[Serializable]
	public class ReactionRule{
		private String[] reactants, products;
		public String From, To;

		public GameObject Particles;
		private GameObject instantiatedParticles;

		public String[] Reactants(){
			if(reactants == null){
				reactants = From.Split(',');
				for(int i = 0; i < reactants.Length;i++)reactants[i] = reactants[i].Trim ();
			}
			return reactants;
		}

		public String[] Products(){
			if(products == null){
				products = To.Split(',');
				for(int i = 0; i < products.Length;i++)products[i] = products[i].Trim ();
			}
			return products;
		}

		public void StartParticles(Transform transform){
			if(Particles == null)return;
			//Particles.SetActive (true);
			instantiatedParticles = (GameObject)GameObject.Instantiate (Particles, transform.position, Quaternion.identity);
			instantiatedParticles.transform.SetParent (transform);
		}

		public void StopParticles (){
			if(instantiatedParticles == null)return;
			//Particles.SetActive (false);
			foreach(ParticleEmitter emitter in instantiatedParticles.GetComponentsInChildren<ParticleEmitter>())
				emitter.emit = false;
			GameObject.Destroy (instantiatedParticles,3f);
			instantiatedParticles = null;
		}
	}

	public ReactionTable.ElementLookup[] m_AllElements;
	public ReactionTable.ReactionRule[] m_AllRules;

	private String SanitizedName(Transform e){
		String name = e.gameObject.name;
		if(name.Contains("(Clone)"))name = name.Remove(name.IndexOf("(Clone)"));
		return name;
	}

	public ReactionRule CanReact(Transform[] elems){
		foreach(ReactionRule rule in m_AllRules){
			if(CanReact(elems,rule))return rule;
		}
		return null;
	}

	private bool CanReact(Transform[] elems, ReactionRule rule){
		bool[] used = new bool[elems.Length];
		foreach(String req in rule.Reactants()){
			bool found = false;
			for(int i = 0;i<elems.Length;i++){
				Transform e = elems[i];
				if(e == null)continue;
				if(SanitizedName(e) != req)continue;
				if(used[i])continue;
				found = true;
				used[i] = true;
				break;
			}
			if(!found)return false;
		}
		return true;
	}

	public IEnumerable<int> GetConsumed(Transform[] elems, ReactionRule rule){
		bool[] used = new bool[rule.Reactants().Length];
		for(int i = elems.Length - 1; i>=0;i--){
			Transform e = elems[i];
			if(e == null)continue;
			for(int j = rule.Reactants().Length - 1; j >= 0 ; j--){
				String req = rule.Reactants()[j];

				if(SanitizedName(e) != req)continue;
				if(used[j])continue;
				
				used[j] = true;
				yield return i;
				break;
			}
		}
	}

	public GameObject GetSpawned(String name){
		foreach(ElementLookup lookup in m_AllElements){
			if(lookup.name == name)return lookup.prefab;
		}
		return null;
	}
}