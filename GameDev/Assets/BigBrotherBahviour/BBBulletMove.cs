using UnityEngine;
using System.Collections;


public class BBBulletMove : MonoBehaviour {

    GameObject player;
    GameObject bigBrother;

    float xSpeed;
    float ySpeed;
    public bool check;
    Vector2 newPosition, movementVector;

    void Start()
    {
        bigBrother = GameObject.Find("BigBrother(Clone)").gameObject;
        player = GameObject.Find("Player").gameObject;
        movementVector = new Vector2(0f, 0f);
        newPosition = new Vector2(0f, 0f);
        check = false;
    }

    void Update()
    {
        

        checkPosition();

        if (!check)
        {
            xSpeed = (player.transform.position.x - bigBrother.transform.position.x)/20;
            ySpeed = (player.transform.position.y - bigBrother.transform.position.y)/20;
            print(player.transform.position.x);
            print(bigBrother.transform.position.x);
            check = true;
        }

        movementVector = new Vector2(xSpeed, ySpeed);
        newPosition = (Vector2)gameObject.transform.position + movementVector;
        gameObject.GetComponent<Rigidbody2D>().MovePosition(newPosition);

        


    }

    void checkPosition()
    {
        if (gameObject.transform.position.x < -12f)
        {
            Destroy(gameObject, 0.05f);
        }
    }

    

   



}
