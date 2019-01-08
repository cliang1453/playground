using UnityEngine;
using System.Collections;

public class BigBrotherMove : MonoBehaviour {
    private float moveSpeed;
    GameObject frameController;
    GameObject player;
    public bool hitLeftEdge;
    private float levelUpTime;
    int time;

    // Use this for initialization
    void Start()
    {
        frameController = GameObject.Find("frameAndDistanceController").gameObject;
        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
        player = GameObject.Find("Player").gameObject;
        moveSpeed = -0.01f;
        hitLeftEdge = false;
        levelUpTime = 10f;
    }

    // Update is called once per frame
    void Update()
    {
        checkPosition();
        checkState();
        

        if (Time.timeSinceLevelLoad > 18 * levelUpTime)
        {
            if(time % 1200 <= 600)
            {
                moveSpeed = -0.02f;
            }
            else if(time % 1200 <= 900)
            {
                moveSpeed = 0.01f;
            }
            else
            {
                moveSpeed = 0f;
            }
        }
        else if (Time.timeSinceLevelLoad > 8 * levelUpTime)
        {
            moveSpeed = -0.02f;
            if (time % 600 >= 300)
            {
                moveSpeed = 0f;
            }
        }
        else
        {
            moveSpeed = -0.01f;
            if (time % 600 >= 400)
            {
                moveSpeed = 0f;
            }
        }
        
         gameObject.transform.Translate(moveSpeed, 0f, 0f);
         time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }

    void checkPosition()
    {
        if(gameObject.transform.position.x <-10f)
        {
            Destroy(gameObject, 0.05f);
        }
    }

    void checkState()
    {
        if (gameObject.transform.position.x < (player.transform.position.x - 1.5f))
        {
            gameObject.GetComponent<BigBrotherSpriteControl>().bbStates = BigBrotherSpriteControl.BigBrotherStates.lookRight;
        }

        else if (gameObject.transform.position.x > (player.transform.position.x + 1.5f))
        {
            gameObject.GetComponent<BigBrotherSpriteControl>().bbStates = BigBrotherSpriteControl.BigBrotherStates.lookLeft;
        }
        else
        {
            gameObject.GetComponent<BigBrotherSpriteControl>().bbStates = BigBrotherSpriteControl.BigBrotherStates.normal;
        }
    }

}
