using UnityEngine;
using System.Collections;

public class BigBrotherSpawn : MonoBehaviour {

    public GameObject spawn;
    GameObject level;
    GameObject frameController;

    bool justSpawn;//to prevent continous to generation
    private enum spawnSpeeds {low, medium, high };
    private spawnSpeeds mySpeed;
    private float levelUpTime;

    int time;

    // Use this for initialization
    void Start()
    {
        Random.seed = Random.Range(0, 30);//set a random seed
        level = GameObject.Find("Level").gameObject;
        levelUpTime = 10f;
        mySpeed = spawnSpeeds.low;
        justSpawn = false;

        frameController = GameObject.Find("frameAndDistanceController").gameObject;
        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }

    // Update is called once per frame
    void Update()
    {
        LevelUp();

        if(mySpeed==spawnSpeeds.low)
        {
            if (time % 2600 <200)
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

        else if(mySpeed == spawnSpeeds.medium)
        {
            if ((time - 5400) % 1200 < 200)
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
            if ((time - 12150) % 700 < 200)
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
        }

        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }

    bool RandomGen()
    {
        int temp = Random.Range(0, 3000);
        return (temp % 100 < 3);

    }

    void LevelUp()
    {
        if (Time.timeSinceLevelLoad > 18 * levelUpTime)
        {
            mySpeed = spawnSpeeds.high;
            
        }
        else if (Time.timeSinceLevelLoad > 8 * levelUpTime)
        {
            mySpeed = spawnSpeeds.medium;
        }
        else
        {
            mySpeed = spawnSpeeds.low;
        }

    }


}
//