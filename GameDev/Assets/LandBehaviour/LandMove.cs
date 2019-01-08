using UnityEngine;
using System.Collections;

public class LandMove : MonoBehaviour {
    GameObject landSpawner;

    // Use this for initialization
    void Start()
    {
        landSpawner = GameObject.Find("landSpawner").gameObject;
    }

    // Update is called once per frame
    void Update()
    {
        gameObject.transform.Translate(landSpawner.GetComponent<LandSpawn>().speed, 0f, 0f);
        if (gameObject.transform.position.x <= -9f)
        {
            Destroy(gameObject);
        }
    }
}
