using UnityEngine;
using System.Collections;

public class upperBehaviour : MonoBehaviour
{
    GameObject target;
    float lastTime;
    public bool readyToSpawn;
    public bool justSpawn;
    GameObject landSpawner;
    // Use this for initialization


    void Start()
    {
        lastTime = Time.time;
        readyToSpawn = false;
        justSpawn = true;
        landSpawner = GameObject.Find("landSpawner").gameObject;
    }

    // Update is called once per frame
    void Update()
    {
        target = GameObject.Find("Player").gameObject;
        checkPosition();
        checkState();
        vertMovement();
    }

    void checkPosition()
    {
        if (gameObject.transform.position.x < -9f)
        {
            Destroy(gameObject, 0.05f);
        }
    }

    void checkState()
    {

        if (gameObject.transform.position.x < (target.transform.position.x - 1.5f))
        {
            gameObject.GetComponent<LittleBrotherSpriteControl>().lbStates = LittleBrotherSpriteControl.LittleBrotherStates.lookRight;
        }

        else if (gameObject.transform.position.x > (target.transform.position.x + 1.5f))
        {
            gameObject.GetComponent<LittleBrotherSpriteControl>().lbStates = LittleBrotherSpriteControl.LittleBrotherStates.lookLeft;
        }
        else
        {
            gameObject.GetComponent<LittleBrotherSpriteControl>().lbStates = LittleBrotherSpriteControl.LittleBrotherStates.normal;
        }
    }

    void vertMovement()
    {
        gameObject.transform.Translate(new Vector3(landSpawner.GetComponent<LandSpawn>().speed, 0, 0));

        if (Time.time > lastTime)
        {
            if (Time.time - lastTime > 4f)
            {
                lastTime = Time.time;
            }
            else
            {
                if (gameObject.transform.position.y <= -0.8f)
                {
                    gameObject.transform.Translate(new Vector3(0, 0.03f, 0));
                }
                else
                {
                    if (gameObject.transform.position.y != target.transform.position.y)
                    {
                        readyToSpawn = false;
                        if (gameObject.transform.position.y > (target.transform.position.y + 0.2)) gameObject.transform.Translate(new Vector3(0, -.03f, 0));
                        else if (gameObject.transform.position.y < (target.transform.position.y - 1.3)) gameObject.transform.Translate(new Vector3(0, 0.03f, 0));
                    }
                    else
                    {
                        readyToSpawn = true;
                        gameObject.transform.Translate(new Vector3(0, 0, 0)); 
                    }
                }
            }
        }
    }
}
