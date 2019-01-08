using UnityEngine;
using System.Collections;

public class LittleBrotherBehaviorScript : MonoBehaviour {
    GameObject target;
    float lastTime;
    public bool readyToSpawn;
    GameObject landSpawner;
    // Use this for initialization


    void Start() {
        lastTime = Time.time;
        readyToSpawn = false;
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
        if (gameObject.transform.position.x < -10f)
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
                if (gameObject.transform.position.y >= 0.8f)
                {
                    gameObject.transform.Translate(new Vector3(0, -.03f, 0));
                }
                else
                {
                    if (gameObject.transform.position.y != target.transform.position.y)
                    {
                        if (gameObject.transform.position.y > (target.transform.position.y + 0.2)) gameObject.transform.Translate(new Vector3(0, -.03f, 0));
                        else if (gameObject.transform.position.y < (target.transform.position.y - 0.2)) gameObject.transform.Translate(new Vector3(0, .03f, 0));
                        else readyToSpawn = true;
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
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*void horiMovement()
    {
        position = (Vector2)gameObject.transform.position + movementVectorX;
        gameObject.GetComponent<Rigidbody2D>().MovePosition(position);
    }

    void vertMovement()
    {
        //one iteration begins
        if (Time.time - lastTime > 0)
        {
            if (Time.time - lastTime > 5f)
            {
                lastTime = Time.time;
            }

            else if (Time.time - lastTime > 3.5f) // stop the iteration 
            {
                position = (Vector2)gameObject.transform.position + movementVectorYdown;
                gameObject.GetComponent<Rigidbody2D>().MovePosition(position);
                gameObject.transform.Translate(movementVectorYdown);
                readyToSpawn = false;
            }

            else
            {
                //the lower lb should not go higher than the middle line of the scene 
                if (gameObject.transform.position.y >= 0.5f)
                {
                    position = (Vector2)gameObject.transform.position + movementVectorYdown;
                    gameObject.GetComponent<Rigidbody2D>().MovePosition(position);
                    gameObject.transform.Translate(movementVectorYdown);
                }
                //if the lower lb is lower than the middle line
                else
                {
                    if (gameObject.transform.position.y > target.transform.position.y + 0.5f)
                    {
                        position = (Vector2)gameObject.transform.position + movementVectorYdown;
                        gameObject.GetComponent<Rigidbody2D>().MovePosition(position);
                        gameObject.transform.Translate(movementVectorYdown);
                    }
                    else if (gameObject.transform.position.y < target.transform.position.y - 0.5f)
                    {
                        position = (Vector2)gameObject.transform.position + movementVectorYup;
                        gameObject.GetComponent<Rigidbody2D>().MovePosition(position);
                        gameObject.transform.Translate(movementVectorYup);
                    }
                    else
                    {
                        position = (Vector2)gameObject.transform.position + movementVectorX;
                        gameObject.GetComponent<Rigidbody2D>().MovePosition(position);
                        gameObject.transform.Translate(movementVectorX);
                        readyToSpawn = true;
                    }
                }
            }
        }
       
        
    }

    void vertMovement()
    {
        if (Time.time - lastTime > 0.5f)
        {
            if (Time.time - lastTime > 5f)
            {
                lastTime = Time.time;
            }

            else if (Time.time - lastTime > 3f) // stop the iteration 
            {
                gameObject.GetComponent<Rigidbody2D>().isKinematic = false;
                readyToSpawn = false;
            }

            else
            {
                gameObject.GetComponent<Rigidbody2D>().isKinematic = false;
                target = GameObject.Find("Player").gameObject;
                Vector2 myForce = new Vector2(0, (target.transform.position.y - gameObject.transform.position.y));
                if (((target.transform.position.y - gameObject.transform.position.y >= 0.5f) && (gameObject.transform.position.y <= 0.3)) && (gameObject.transform.position.y >= -4.35f))
                {
                    if (myForce.magnitude > 1.2)
                    {
                        gameObject.GetComponent<Rigidbody2D>().AddForce(myForce / 3, ForceMode2D.Impulse);
                    }
                }
                else //when the lb reach a stage that it moves on the same level as the player
                {
                    gameObject.GetComponent<Rigidbody2D>().isKinematic = true;
                    //gameObject.transform.Translate(0f, 0f, 0f);
                    readyToSpawn = true;
                }
            }
        }
    }*/
}

