using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class PlayerTrigger : MonoBehaviour {

    public List<GameObject> bullets;
    public AudioClip bb;
    public AudioClip lb;
    public AudioClip die;
    public AudioClip heart;
    
    //public AudioClip swirl;
    public bool shotbyBB;
    public bool shotbyLB;
    public bool hitLava;
    public bool collectHeart;
    public int hitSwirl;


    void Start () {
        bullets = new List<GameObject>();
        hitSwirl = 0;
        shotbyBB = false;
        shotbyLB = false;
        hitLava = false;
        collectHeart = false;
        
    }
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnTriggerExit2D(Collider2D theOtherObjectsCollider)
    {
        gameObject.transform.parent = null;
    }

    void OnTriggerEnter2D(Collider2D theOtherObjectsCollider)
    {
        if (theOtherObjectsCollider.gameObject.name == "bbBullet(Clone)")
        {
            shotbyBB = true;
            gameObject.GetComponent<AudioSource>().clip = bb;
            gameObject.GetComponent<AudioSource>().Play();
            theOtherObjectsCollider.GetComponent<SpriteRenderer>().enabled = false;
            theOtherObjectsCollider.GetComponent<Collider2D>().enabled = false;
            Destroy(theOtherObjectsCollider, 0.05f);
        }

        else if (theOtherObjectsCollider.gameObject.name == "lbBullet(Clone)")
        {
            shotbyLB = true;
            gameObject.GetComponent<AudioSource>().clip = lb;
            gameObject.GetComponent<AudioSource>().Play();
            theOtherObjectsCollider.GetComponent<SpriteRenderer>().enabled = false;
            theOtherObjectsCollider.GetComponent<Collider2D>().enabled = false;
            bullets.Remove(theOtherObjectsCollider.gameObject);
            Destroy(theOtherObjectsCollider, 0.05f);
        }


        else if (theOtherObjectsCollider.gameObject.name == "Lava")
        {
            hitLava = true;
            gameObject.GetComponent<AudioSource>().clip = die;
            gameObject.GetComponent<AudioSource>().Play();
        }

        else if(theOtherObjectsCollider.gameObject.name == "HeartSpawned(Clone)")
        {
            collectHeart = true;
            gameObject.GetComponent<AudioSource>().clip = heart;
            gameObject.GetComponent<AudioSource>().Play();
            theOtherObjectsCollider.GetComponent<SpriteRenderer>().enabled = false;
            theOtherObjectsCollider.GetComponent<Collider2D>().enabled = false;
            Destroy(theOtherObjectsCollider, 0.05f);
        }

        else if ((theOtherObjectsCollider.gameObject.name == "swirl(Clone)")&&(hitSwirl<2))
        {
            //gameObject.GetComponent<AudioSource>().clip = swirl;
            //gameObject.GetComponent<AudioSource>().Play();
            theOtherObjectsCollider.GetComponent<SpriteRenderer>().enabled = false;
            theOtherObjectsCollider.GetComponent<Collider2D>().enabled = false;
            Destroy(theOtherObjectsCollider, 0.05f);
            hitSwirl++;
            print("playertrigger script is change hitSwirl to be" + hitSwirl);
        }

        else
        {
            gameObject.transform.parent = theOtherObjectsCollider.gameObject.transform;
        }
    }
}
