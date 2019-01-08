using UnityEngine;
using System.Collections;

public class BigBrotherSpriteControl : MonoBehaviour {

    public enum BigBrotherStates { normal, lookRight, lookLeft };
    public BigBrotherStates bbStates;
    public Sprite normalSprite;
    public Sprite lookRightSprite;
    public Sprite lookLeftSprite;

    void Start () {
        bbStates = BigBrotherStates.normal;
	}
	
	// Update is called once per frame
	void Update () {
        if (bbStates==BigBrotherStates.normal)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = normalSprite;
        }
        else if (bbStates == BigBrotherStates.lookLeft)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = lookLeftSprite;
        }
        else if (bbStates == BigBrotherStates.lookRight)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = lookRightSprite;
        }
    }
}
