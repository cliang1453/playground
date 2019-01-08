using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class LBBulletSpawn : MonoBehaviour {
    public List<GameObject> bullets;
    public GameObject lbbullet;
    GameObject bullet;
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
    }

    void Update()
    {
        if (littleBrother.GetComponent<LittleBrotherBehaviorScript>().readyToSpawn)
        {
            if (RandomGen() && (!justSpawn))
            {
                bullet = (GameObject)Instantiate(lbbullet, gameObject.transform.position, Quaternion.identity);
                bullets.Add(bullet);

                if(bullet.transform.position.x > player.transform.position.x)
                {
                    bullet.GetComponent<rightMove>().enabled = false;
                    bullet.GetComponent<leftMove>().enabled = true;
                }

                else if (bullet.transform.position.x <= player.transform.position.x)
                {
                    bullet.GetComponent<rightMove>().enabled = true;
                    bullet.GetComponent<leftMove>().enabled = false;
                }

                littleBrother.GetComponent<LittleBrotherBehaviorScript>().readyToSpawn = false;
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