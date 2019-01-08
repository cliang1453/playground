using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class frameAndDistanceController : MonoBehaviour {
    public int frameCount;
    public GameObject heart;
    public int heartCount;
    public int heartCollected;
    public int bbBullet;
    public int lbBullet;
    public int time;
    GameObject player;
    GameObject scoreCounter;
    public List<GameObject> hearts;
    // Use this for initialization
    void Start () {
        frameCount = 0;
        heartCount = 10;
        heartCollected = 0;
        bbBullet = 0;
        lbBullet = 0;
        time = 0;

        player = GameObject.Find("Player").gameObject;
        hearts = new List<GameObject>();
        scoreCounter = GameObject.Find("ScoreCounter").gameObject;
        for (int i=0; i<10; i++)
        {
            hearts.Add((GameObject)Instantiate(heart, new Vector3(-7.5f, 1.6f-i*0.5f, 0f), Quaternion.identity));
            print("Add" + (i + 1) + "heart");
        }
    }
	
	// Update is called once per frame
	void Update () {
        if(player.GetComponent<PlayerTrigger>().shotbyBB)
        {
            if(heartCount==1)
            {
                Destroy(hearts[heartCount - 1], 0.05f);
                hearts.Remove(hearts[heartCount - 1]);
                heartCount = heartCount - 1;
                bbBullet++;
                player.GetComponent<PlayerTrigger>().shotbyBB = false;
            }
            else
            {
                // update heart count and destroy heart objects
                Destroy(hearts[heartCount - 1], 0.05f);
                Destroy(hearts[heartCount - 2], 0.05f); //delete two hearts*/
                hearts.Remove(hearts[heartCount - 1]);
                hearts.Remove(hearts[heartCount - 2]);
                heartCount = heartCount - 2;//update heart count
                bbBullet++;
                player.GetComponent<PlayerTrigger>().shotbyBB = false;
            }
        }

        if (player.GetComponent<PlayerTrigger>().shotbyLB)
        {
            // update heart count and destroy heart objects
            Destroy(hearts[heartCount - 1], 0.05f);
            hearts.Remove(hearts[heartCount - 1]);
            heartCount = heartCount - 1;//update heart count
            lbBullet++;
            player.GetComponent<PlayerTrigger>().shotbyLB = false;
        }

        if (player.GetComponent<PlayerTrigger>().collectHeart)
        {
            // update heart count and destroy heart objects
            heartCount = heartCount + 1;//update heart count
            heartCollected++;
            hearts.Add((GameObject)Instantiate(heart, new Vector3(-7.5f, 1.6f - (heartCount-1) * 0.5f, 0f), Quaternion.identity));
            player.GetComponent<PlayerTrigger>().collectHeart = false;
        }

        frameCount++;
        time = (int) Time.time;
        scoreCounter.GetComponent<TextMesh>().text = "You survive for " + time + " s!";

        //the score should include time, bbBullet shot, lbBullet shot, heart collected, which level the player acheived
    }
}
