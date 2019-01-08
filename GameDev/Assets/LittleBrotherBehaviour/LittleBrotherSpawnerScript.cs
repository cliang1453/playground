using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class LittleBrotherSpawnerScript : MonoBehaviour {

    public GameObject littleBrother;
    GameObject level;
    GameObject frameController;
    GameObject player;

    private enum spawnSpeeds { low, medium, high };
    private spawnSpeeds mySpeed;
    private float levelUpTime;

    public int unit;
    public float speed;

    int time;

    // Use this for initialization
    void Start()
    {
        level = GameObject.Find("Level").gameObject;
        levelUpTime = 10f;
        mySpeed = spawnSpeeds.low;
        speed = -0.03f;
        player = GameObject.Find("Player").gameObject;
        frameController = GameObject.Find("frameAndDistanceController").gameObject;
        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
}

    // Update is called once per frame
    void Update()
    {
        LevelUp();

        if (mySpeed == spawnSpeeds.low)
        {
            unit = 110;
        }
        else if (mySpeed == spawnSpeeds.medium)
        {
            unit = 94;
        }
        else if (mySpeed == spawnSpeeds.high)
        {
            unit = 80;
        }

        int unitTime = 10 * unit;

        if ((time % unitTime == 2 * unit) || ((time % unitTime == 4 * unit) || (time % unitTime == 7 * unit)))
        {
               Instantiate(littleBrother, gameObject.transform.position, Quaternion.identity);//instantiate each guests
        }

        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }

   

    void LevelUp()
    {
        if ((Time.timeSinceLevelLoad > 18 * levelUpTime)||(player.GetComponent<PlayerTrigger>().hitSwirl==2))
        {
            level.GetComponent<Level>().GetComponent<TextMesh>().text = "Level 3";
            mySpeed = spawnSpeeds.high;
            speed = -0.042f;
        }
        else if ((Time.timeSinceLevelLoad > 8 * levelUpTime)||(player.GetComponent<PlayerTrigger>().hitSwirl==1))
        {
            print(player.GetComponent<PlayerTrigger>().hitSwirl);
            level.GetComponent<Level>().GetComponent<TextMesh>().text = "Level 2";
            mySpeed = spawnSpeeds.medium;
            speed = -0.036f;
        }
        else
        {
            level.GetComponent<Level>().GetComponent<TextMesh>().text = "Level 1";
            mySpeed = spawnSpeeds.low;
            speed = -0.030f;
            
        }

    }
}





















/*
 bool RandomGen()
    {
        int temp = Random.Range(0, 3000);
        return (temp % 100 < 3);

    }
    
time = frameController.GetComponent<frameAndDistanceController>().frameCount;
if (mySpeed == spawnSpeeds.low)
{
    if ((time % 100 > 85) || ((time % 100 > 35) && (time % 100 < 55))) // 10 percent generation rate 
    {
        if (RandomGen() && (!justSpawn))
        {
            Instantiate(spawn, gameObject.transform.position, Quaternion.identity);//instantiate each guests
            justSpawn = true;
        }
    }
    else
    {
        justSpawn = false;
    }
}

else if (mySpeed == spawnSpeeds.medium)
{
    if ((((time-5400) % 100 > 10) && ((time - 5400) % 100 < 25)) || (((time - 5400) % 100 > 50) && ((time - 5400) % 100 < 65))) //30 percent
    {
        if (RandomGen() && (!justSpawn))
        {
            Instantiate(spawn, gameObject.transform.position, Quaternion.identity);//instantiate each guests
            justSpawn = true; // prevent spawn together
        }
    }
    else
    {
        justSpawn = false;
    }
}


else if (mySpeed == spawnSpeeds.high)
{
    if ((((time - 12150) % 100 > 20) && ((time - 12150) % 100 < 40)) || (((time - 12150) % 100 > 60) && ((time - 12150) % 100 < 80))) // 40 percent
    {
        if (RandomGen() && (!justSpawn))
        {
            Instantiate(spawn, gameObject.transform.position, Quaternion.identity);//instantiate each guests
            justSpawn = true;//prevent spawn together
        }
    }
    else
    {
        justSpawn = false;
    }
}*/















/*float lastTime;
public GameObject spawn;

void Start () {

    lastTime = Time.time;
}

// Update is called once per frame
void Update () {
    if (Time.time - lastTime > 3)
    {
        Instantiate(spawn, gameObject.transform.position, Quaternion.identity);
        lastTime = Time.time;
    }

}
}*/
