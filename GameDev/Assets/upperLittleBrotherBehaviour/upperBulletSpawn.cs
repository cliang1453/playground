using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class upperBulletSpawn : MonoBehaviour
{
    public List<GameObject> bullets;
    public GameObject bullet;
    public GameObject littleBrother;
    public float xSpeed;
    GameObject player;
    bool justSpawn;

    void Start()
    {
        player = GameObject.Find("Player").gameObject;
        bullets = player.GetComponent<PlayerTrigger>().bullets;
        //leftBullets = new List<GameObject>();
        justSpawn = false;

        bullet.GetComponent<rightMove>().enabled = false;
        bullet.GetComponent<leftMove>().enabled = false;
    }

    void Update()
    {
        if (littleBrother.GetComponent<upperBehaviour>().readyToSpawn)
        {
            if (RandomGen() && (!justSpawn))
            {
                bullet = (GameObject)Instantiate(bullet, gameObject.transform.position, Quaternion.identity);//instantiate each guests
                bullets.Add(bullet);

                if (bullet.transform.position.x > player.transform.position.x)
                {
                    bullet.GetComponent<rightMove>().enabled = true;
                    bullet.GetComponent<leftMove>().enabled = false;
                }

                else if (bullet.transform.position.x <= player.transform.position.x)
                {
                    bullet.GetComponent<rightMove>().enabled = false;
                    bullet.GetComponent<leftMove>().enabled = true;
                }

                littleBrother.GetComponent<upperBehaviour>().readyToSpawn = false;
                justSpawn = true;
            }
        }
        else
        {
            justSpawn = false;
        }
    }


    bool RandomGen()
    {
        int temp = Random.Range(0, 3000);
        return (temp % 1000 < 5);
    }
}
