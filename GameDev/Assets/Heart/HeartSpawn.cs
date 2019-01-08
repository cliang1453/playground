using UnityEngine;
using System.Collections;

public class HeartSpawn : MonoBehaviour {
    public GameObject heart;
    public GameObject littleBrother;
    bool justSpawn;
    float lastTime;

    void Start()
    {
        lastTime = Time.time;
        justSpawn = false;
    }

    void Update()
    {
        //only instantiate at the steady state 
        
        if(Time.time > lastTime)
        {
            if(Time.time - lastTime >= 500f)
            {
                lastTime = Time.time;
                justSpawn = false;
            }

            if (!justSpawn && RandomGen())
            {
                GameObject newHeart = (GameObject)Instantiate(heart, gameObject.transform.position, Quaternion.identity);
                newHeart.transform.parent = gameObject.transform;

                justSpawn = true;
            }
        }

       
    }

    bool RandomGen()
    {
        int temp = Random.Range(0, 3000);
        return (temp % 10000 < 5);
    }
}