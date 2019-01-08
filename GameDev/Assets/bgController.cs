using UnityEngine;
using System.Collections;

public class bgController : MonoBehaviour {
    public GameObject background;
    float lastTime;
    GameObject player;
    GameObject newBackground;
    public AudioClip bg;
    public Sprite level1;
    public Sprite level2;
    public Sprite level3;
    // Use this for initialization
    void Start () {
        lastTime = 0;
        player = GameObject.Find("Player").gameObject;
        //background = GameObject.Find("bg").gameObject;
        newBackground = (GameObject)Instantiate(background, new Vector2(11f, 0f), Quaternion.identity);
        gameObject.GetComponent<AudioSource>().clip = bg;
        gameObject.GetComponent<AudioSource>().Play();
        
    }
	
	// Update is called once per frame
	void Update () {

        if (Time.time - lastTime >= 12f)
        {
            Destroy(newBackground, 0.05f);
            newBackground = (GameObject)Instantiate(background, new Vector2(11f, 0f), Quaternion.identity);
            lastTime = Time.time;
        }

        newBackground.transform.Translate(new Vector2(-0.03f, 0f));

        if (player.GetComponent<PlayerTrigger>().hitSwirl== 0)
        {
            newBackground.GetComponent<SpriteRenderer>().sprite = level1;
        }
        if (player.GetComponent<PlayerTrigger>().hitSwirl == 1)
        {
            newBackground.GetComponent<SpriteRenderer>().sprite = level2;
        }
        if (player.GetComponent<PlayerTrigger>().hitSwirl == 2)
        {
            newBackground.GetComponent<SpriteRenderer>().sprite = level3;
        }

    }
}
