using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class PlayerSpriteControl : MonoBehaviour {

    public enum PlayerStates { moveRight, normal, jump, getShot, almostDie};
    public PlayerStates playerState;

    public Sprite normalSprite;
    public Sprite jumpSprite;
    public Sprite getShotSprite;
    public Sprite almostDieSprite; //This is the case that the player only have one heart left
    public List<Sprite> walkingSprites = new List<Sprite>(3);
    // Use this for initialization
    void Start()
    {
        playerState = PlayerStates.normal;
    }

    // Update is called once per frame
    void Update()
    {
        print(playerState);

        if (playerState == PlayerStates.moveRight)
        {
            int index = gameObject.GetComponent<PlayerMove>().count;
            if (index <= 29)
            {
                index /= 5;
                gameObject.GetComponent<SpriteRenderer>().sprite = walkingSprites[index];
            }
            else
            {
                gameObject.GetComponent<PlayerMove>().count = 0;
                index = 0;
            }
        }
        else if (playerState == PlayerStates.normal)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = normalSprite;
        }
        else if (playerState == PlayerStates.jump)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = jumpSprite;
        }
        else if(playerState == PlayerStates.getShot)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = getShotSprite;
        }
        else if (playerState == PlayerStates.almostDie)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = almostDieSprite;
        }
    }
}
