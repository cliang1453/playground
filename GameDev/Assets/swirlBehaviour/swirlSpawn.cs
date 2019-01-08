using UnityEngine;
using System.Collections;

public class swirlSpawn : MonoBehaviour {
    float lastTime;
    public GameObject swirl;
    GameObject player;
    Vector2 position;
	// Use this for initialization
	void Start () {
        lastTime = 0;
        player = GameObject.Find("Player").gameObject;
    }
	
	// Update is called once per frame
	void Update () {
        if(Time.time < 70f)
        {
            //print("the time difference" + (Time.time - lastTime));
            if (Time.time - lastTime >= 25f)
            {
                position = new Vector2(player.transform.position.x+2f, player.transform.position.y+2f);
                GameObject newSwirl = (GameObject)Instantiate(swirl,position, Quaternion.identity);
                //newSwirl.transform.parent = gameObject.transform;
                lastTime = Time.time;
            }
        }
    }
}
