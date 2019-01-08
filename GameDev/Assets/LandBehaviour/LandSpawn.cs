using UnityEngine;
using System.Collections;

public class LandSpawn : MonoBehaviour {

    public GameObject spawnLong;
    public GameObject spawnShort;
    GameObject frameController;
    bool result;

    public enum spawnSpeeds { low, medium, high };
    public spawnSpeeds mySpeed;
    private float levelUpTime;
    public int unit;
    public float speed;
    GameObject player;

    int time;

    // Use this for initialization
    void Start()
    {
        Random.seed = Random.Range(0, 30);//set a random seed
        levelUpTime = 10f;
        mySpeed = spawnSpeeds.low;
        speed = -0.03f;
        frameController = GameObject.Find("frameAndDistanceController").gameObject;
        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
        player = GameObject.Find("Player").gameObject;
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

        result = randomLand();
        int unitTime = 10 * unit;
        if (((time % unitTime == 0) || (time % unitTime == unit)) || ((time % unitTime == 6 * unit) || (time % unitTime == 9 * unit)))
        {
                if (result) // result = true, spawn short land 
                {
                    Instantiate(spawnShort, gameObject.transform.position, Quaternion.identity);//instantiate each guests
                }
                else // result = false, spawn long land 
                {
                    Instantiate(spawnLong, gameObject.transform.position, Quaternion.identity);//instantiate each guests
                }
        }

        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }


        /*else if (mySpeed == spawnSpeeds.medium)
        {
            if ((((time - 5400) % 100 > 35) && ((time - 5400) % 100 < 45)) || (((time - 5400) % 100 > 85) && ((time - 5400) % 100 < 95))) //20 percent
            {
                if (RandomGen() && (!justSpawn))
                {
                    if (result)
                    {
                        Instantiate(spawnShort, gameObject.transform.position, Quaternion.identity);//instantiate each guests
                    }
                    else
                    {
                        Instantiate(spawnLong, gameObject.transform.position, Quaternion.identity);//instantiate each guests
                    }

                    justSpawn = true;
                }
            }
            else
            {
                justSpawn = false;
            }
        }


        else if (mySpeed == spawnSpeeds.high)
        {
            if ((((time - 12150) % 100 > 0) && ((time - 12150) % 100 < 10)) || (((time - 12150) % 100 > 55) && ((time - 12150) % 100 < 60))) //15
            {
                if (RandomGen() && (!justSpawn))
                {
                    if (result)
                    {
                        Instantiate(spawnShort, gameObject.transform.position, Quaternion.identity);//instantiate each guests
                    }
                    else
                    {
                        Instantiate(spawnLong, gameObject.transform.position, Quaternion.identity);//instantiate each guests
                    }

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
        return (temp % 100 < 3);

    }*/
    
    bool randomLand()
    {
        int temp = Random.Range(0, 3000);
        if (temp % 100 > 45)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void LevelUp()
    {
        if ((Time.timeSinceLevelLoad > 18 * levelUpTime)||(player.GetComponent<PlayerTrigger>().hitSwirl == 2))
        {
            mySpeed = spawnSpeeds.high;
            speed = -0.042f;
        }
        else if ((Time.timeSinceLevelLoad > 8 * levelUpTime)|| (player.GetComponent<PlayerTrigger>().hitSwirl == 1))
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













