using UnityEngine;
using System.Collections;

public class LittleBrotherSpriteControl : MonoBehaviour {

    public enum LittleBrotherStates { normal, lookRight, lookLeft };
    public LittleBrotherStates lbStates;
    public Sprite normalSprite;
    public Sprite lookRightSprite;
    public Sprite lookLeftSprite;

    void Start()
    {
        lbStates = LittleBrotherStates.normal;
    }

    // Update is called once per frame
    void Update()
    {
        if (lbStates == LittleBrotherStates.normal)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = normalSprite;
        }
        else if (lbStates == LittleBrotherStates.lookLeft)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = lookLeftSprite;
        }
        else if (lbStates == LittleBrotherStates.lookRight)
        {
            gameObject.GetComponent<SpriteRenderer>().sprite = lookRightSprite;
        }
    }
}
