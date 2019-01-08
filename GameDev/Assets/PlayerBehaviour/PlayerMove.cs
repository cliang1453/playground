using UnityEngine;
using System.Collections;

public class PlayerMove: MonoBehaviour {
    public float playerRightSpeed=1f;
    public float playerJumpSpeed=0.5f;
    public int count;
    // Use this for initialization
    void Start () {
        count = 0;
	}
	
	// Update is called once per frame
	void Update () {

       if(GetComponent<PlayerSpriteControl>().playerState != PlayerSpriteControl.PlayerStates.almostDie)
       {
            if (Input.GetKey(KeyCode.RightArrow))
            {
                if (Input.GetKeyUp(KeyCode.UpArrow))
                {
                    gameObject.transform.Translate(playerRightSpeed, playerJumpSpeed, 0f);
                    GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.jump;
                }
                else
                {
                    count++;
                    gameObject.transform.Translate(playerRightSpeed, 0f, 0f);
                    GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.moveRight;
                }
            }

            else if (Input.GetKey(KeyCode.LeftArrow))
            {

                if (Input.GetKeyUp(KeyCode.UpArrow))
                {
                    gameObject.transform.Translate(-playerRightSpeed, playerJumpSpeed, 0f);
                    GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.jump;
                }
                else
                {
                    count++;
                    gameObject.transform.Translate(-playerRightSpeed, 0f, 0f);
                    GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.moveRight;
                }
            }

            else if (Input.GetKeyUp(KeyCode.UpArrow))
            {
                gameObject.transform.Translate(0, playerJumpSpeed, 0f);
                GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.jump;
            }

            else
            {
                gameObject.transform.Translate(0f, 0f, 0f);
                GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.normal;
            }
        }



        else
        {
            if (Input.GetKey(KeyCode.RightArrow))
            {
                if (Input.GetKeyUp(KeyCode.UpArrow))
                {
                    gameObject.transform.Translate(playerRightSpeed, playerJumpSpeed, 0f);
                    GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.jump;
                }
                else
                {
                    count++;
                    gameObject.transform.Translate(playerRightSpeed, 0f, 0f);
                }
            }

            else if (Input.GetKey(KeyCode.LeftArrow))
            {

                if (Input.GetKeyUp(KeyCode.UpArrow))
                {
                    gameObject.transform.Translate(-playerRightSpeed, playerJumpSpeed, 0f);
                    GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.jump;
                }
                else
                {
                    count++;
                    gameObject.transform.Translate(-playerRightSpeed, 0f, 0f);
                }
            }

            else if (Input.GetKeyUp(KeyCode.UpArrow))
            {
                gameObject.transform.Translate(0, playerJumpSpeed, 0f);
                GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.jump;
            }

            else
            {
                gameObject.transform.Translate(0f, 0f, 0f);
            }
        }
    }
}
