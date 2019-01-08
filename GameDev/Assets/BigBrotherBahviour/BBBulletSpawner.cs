using UnityEngine;
using System.Collections;

public class BBBulletSpawner : MonoBehaviour {

    public GameObject bbBullet;
    public GameObject Player;
    //GameObject bBrother;
    GameObject frameController; 
    private float levelUpTime;
    //GameObject bbBulletSpawner;
    int time;

    void Start()
    {
        //bBrother = GameObject.Find("BigBrother(Clone)").gameObject;
        frameController = GameObject.Find("frameAndDistanceController").gameObject;
        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
        //bbBulletSpawner = bBrother.GetComponentInChildren<BBBulletSpawner>().gameObject;
        //bbBulletSpawner = GameObject.Find("BBBulletSpawner(Clone)");
    }

    void Update()
    {
        //bBrother = GameObject.Find("BigBrother(Clone)").gameObject;
        //bbBulletSpawner = bBrother.GetComponentInChildren<BBBulletSpawner>().gameObject;

        if (Time.timeSinceLevelLoad > 18 * levelUpTime)
        {
            if ((time % 400 ==1)||(time % 200 ==1))
            {
                bbBullet.GetComponent<BBBulletMove>().check = false;
                Instantiate(bbBullet, gameObject.transform.position, Quaternion.identity);
                //bbBullet.GetComponent<BBBulletMove>().check = false;
            }   
        }
        else if (Time.timeSinceLevelLoad > 8 * levelUpTime)
        {
            if ((time % 500 == 1) || (time % 300 == 1))
            {
                bbBullet.GetComponent<BBBulletMove>().check = false;
                Instantiate(bbBullet, gameObject.transform.position, Quaternion.identity);
                //bbBullet.GetComponent<BBBulletMove>().check = false;
            }
        }
        else
        {
            if ((time % 600 == 1) || (time % 400 == 1))
            {
                bbBullet.GetComponent<BBBulletMove>().check = false;
                Instantiate(bbBullet, gameObject.transform.position, Quaternion.identity);
            }
        }

        time = frameController.GetComponent<frameAndDistanceController>().frameCount;
    }
}
