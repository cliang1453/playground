using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class upperSpawner : MonoBehaviour
{

    public GameObject littleBrother;
    GameObject level;
    GameObject frameController;

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
        frameController = GameObject.Find("frameAndDistanceController").gameObject;
        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }

    // Update is called once per frame
    void Update()
    {

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

        if ((time % unitTime == 3 * unit) || ((time % unitTime == 5 * unit) || (time % unitTime == 8 * unit)))
        {
            Instantiate(littleBrother, gameObject.transform.position, Quaternion.identity);//instantiate each guests
        }

        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }



    void LevelUp()
    {
        if (Time.timeSinceLevelLoad > 18 * levelUpTime)
        {
            mySpeed = spawnSpeeds.high;
            speed = -0.042f;
        }
        else if (Time.timeSinceLevelLoad > 8 * levelUpTime)
        {
            mySpeed = spawnSpeeds.medium;
            speed = -0.036f;
        }
        else
        {
            mySpeed = spawnSpeeds.low;
            speed = -0.030f;
        }

    }
}