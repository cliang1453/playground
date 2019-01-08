using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

public class LostConditionController : MonoBehaviour
{
    public int heartCount;
    public AudioClip die;
    public GameObject Player;
    public GameObject BigBrother;
    public GameObject LittleBrother;
    public GameObject upperLittleBrother;
    public GameObject longLand;
    public GameObject shortLand;

    GameObject frameController;
    GameObject bgController;
    GameObject landSpawner;
    GameObject upperSpawner;
    GameObject bottomSpawner;
    GameObject loseScript;

    int dieTime;
   

    // Use this for initialization
    void Start()
    {
        frameController = GameObject.Find("frameAndDistanceController").gameObject;
        upperSpawner = GameObject.Find("UpperSpawner").gameObject;
        bottomSpawner = GameObject.Find("BottomSpawner").gameObject;
        landSpawner = GameObject.Find("landSpawner").gameObject;
        bgController = GameObject.Find("swirlController").gameObject;
        loseScript = GameObject.Find("loseScript").gameObject;

        LittleBrother.GetComponent<LittleBrotherBehaviorScript>().enabled = true;
        upperLittleBrother.GetComponent<upperBehaviour>().enabled = true;
        BigBrother.GetComponent<BigBrotherMove>().enabled = true;
        //landSpawner.GetComponent<LandSpawn>().enabled = true;
        //longLand.GetComponent<LandMove>().enabled = true;
       // shortLand.GetComponent<LandMove>().enabled = true;
    }

    // Update is called once per frame
    void Update()
    {
        heartCount = frameController.GetComponent<frameAndDistanceController>().heartCount;

        //Lost Condition 1: When player has no heart left
        if (heartCount < 3)
        { 
            if(heartCount == 0) //die
            {
                dieTime = frameController.GetComponent<frameAndDistanceController>().time;

                

                Player.GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.getShot;
                Player.GetComponent<PlayerMove>().enabled = false;
                Player.GetComponent<PlayerTrigger>().enabled = false;

                landSpawner.GetComponent<LandSpawn>().enabled = false;
                //longLand.GetComponent<LandMove>().enabled = false;
                //shortLand.GetComponent<LandMove>().enabled = false;

                BigBrother.GetComponent<BigBrotherMove>().enabled = false;
                upperSpawner.GetComponent<BigBrotherSpawn>().enabled = false;
                
                LittleBrother.GetComponent<LittleBrotherBehaviorScript>().enabled = false;
                bottomSpawner.GetComponent<LittleBrotherSpawnerScript>().enabled = false;
                upperLittleBrother.GetComponent<upperBehaviour>().enabled = false;
                upperSpawner.GetComponent<upperSpawner>().enabled = false;

                frameController.GetComponent<frameAndDistanceController>().enabled = false;
                bgController.GetComponent<bgController>().enabled = false;

                //Audio
                Player.GetComponent<AudioSource>().clip = die;
                Player.GetComponent<AudioSource>().Play();

                showtext();
                //Invoke("LoadLevel", 3f);
            }

            else if(heartCount>0) //else, almost die
            {
                Player.GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.almostDie;
            }
        }


        //Lost Condition 2: When drop into the Lava
        if (Player.GetComponent<PlayerTrigger>().hitLava)
        {
            dieTime = frameController.GetComponent<frameAndDistanceController>().time;
            LittleBrother.GetComponent<LittleBrotherBehaviorScript>().enabled = false;
            Player.GetComponent<PlayerSpriteControl>().playerState = PlayerSpriteControl.PlayerStates.getShot;
            Player.GetComponent<PlayerMove>().enabled = false;
            Player.GetComponent<PlayerTrigger>().enabled = false;

            landSpawner.GetComponent<LandSpawn>().enabled = false;
            //longLand.GetComponent<LandMove>().enabled = false;
            //shortLand.GetComponent<LandMove>().enabled = false;

            BigBrother.GetComponent<BigBrotherMove>().enabled = false;
            upperSpawner.GetComponent<BigBrotherSpawn>().enabled = false;

           
            bottomSpawner.GetComponent<LittleBrotherSpawnerScript>().enabled = false;
            upperLittleBrother.GetComponent<upperBehaviour>().enabled = false;
            upperSpawner.GetComponent<upperSpawner>().enabled = false;

            frameController.GetComponent<frameAndDistanceController>().enabled = false;
            bgController.GetComponent<bgController>().enabled = false;

            Player.GetComponent<AudioSource>().clip = die;
            Player.GetComponent<AudioSource>().Play();

            showtext();
            //Audio 
            //Invoke("LoadLevel", 3f);
        }
    }

    void showtext()
    {
        loseScript.GetComponent<TextMesh>().text = "\nNew Records! \n    You survive for " + dieTime + " s!\n         Renew the page to restart!\n";
    }

   /* public static void LoadLevel()
    {
        Application.LoadLevel(1);
    }*/
}
