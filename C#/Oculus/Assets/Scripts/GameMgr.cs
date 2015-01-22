using UnityEngine;
using System.Collections;

public enum GameState {
	PLAY = 1, PAUSE = 2,
}

public class GameMgr : MonoBehaviour {

	public static GameState State = GameState.PLAY;
	public bool m_IsUseOculus = false;

	// Use this for initialization
	void Start () {
	
	}
}
