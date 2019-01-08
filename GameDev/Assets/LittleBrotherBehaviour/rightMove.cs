using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class rightMove : MonoBehaviour {

    public List<GameObject> bullets;

    //GameObject bulletSpawner;
    GameObject player;


    void Start()
    {
        //bulletSpawner = GameObject.Find("LBBulletSpawner").gameObject;
        player = GameObject.Find("Player").gameObject;
        bullets = player.GetComponent<PlayerTrigger>().bullets;
    }

    // Update is called once per frame
    void Update () {

        bullets = player.GetComponent<PlayerTrigger>().bullets;

        foreach (GameObject bullet in bullets)//charactors
        {
            if (bullet.transform.position.x > 12f)
            {
                
                bullets.Remove(bullet);
                Destroy(bullet, 0.01f);
                break;
            }

            bullet.transform.Translate(new Vector2(0.3f, 0f));
            bullet.transform.Rotate(new Vector3(20f, 0f, 0f));
        }
    }
}
