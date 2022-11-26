using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using static GameData;
using static GameManager;

public class ItemBody : CopiumScript//, IPointerClickHandler
{
    int thisItemRef;
    public Item thisItem;
    public bool selected = false;
    //TextMeshProUGUI itemName;
    GameObject ItemImage;
    List<GameObject> arrowpoint;

    Vector3 baseScale;
    bool jiggle = false;
    bool jiggle1 = false;
    bool jiggle2 = false;
    float jiggleTimerSet = 0.25f;
    float jiggleTimer = 0f;

    public static bool activeArrow;

    /*public void OnPointerClick(PointerEventData eventData)
    {
        if (thisItem != null)
        {

            if (SelectingItem == false && selected == false)
            {
                selected = true;

                SelectingItem = true;
                SelectedItem = thisItem;
                SelectedItemBody = this;
            }
            else if (SelectingItem == true && selected == true)
            {
                selected = false;

                SelectingItem = false;
                SelectedItem = null;
                SelectedItemBody = null;
            }
            else if (SelectingItem == true && selected == false)
            {
                selected = true;

                SelectedItemBody.selected = false;
                SelectedItem = thisItem;
                SelectedItemBody = this;
            }
        }
        else if (thisItem == null)
        {
            selected = false;

            if (SelectingItem == true)
            {
                SelectedItemBody.selected = false;
            }
            SelectingItem = false;
            SelectedItem = null;
            SelectedItemBody = null;
        }
        activeArrow = false;
    }*/

    // Start is called before the first frame update
    void Start()
    {
        


        baseScale = transform.localScale;
    }

    // Update is called once per frame
    void Update()
    {
        if (jiggle)
        {
            jiggleTimer += Time.deltaTime;

            if (jiggle1 == true)
            {
                //transform.localScale = Vector3.Lerp(baseScale, new Vector3(baseScale.position.x + 0.2f, -0.1f, 0.1f), jiggleTimer);
                transform.localScale = Vector3.Lerp(transform.localScale, (baseScale + new Vector3(0.1f, 0.1f, 0f)), (jiggleTimer * (1 / jiggleTimerSet)));
            }
            if (jiggle2 == true)
            {
                transform.localScale = Vector3.Lerp(transform.localScale, (baseScale + new Vector3(-0.1f, -0.1f, 0f)), (jiggleTimer * (1 / jiggleTimerSet)));
                //transform.localScale = baseScale + new Vector3(-0.1f, 0.05f, 0.1f);
            }
            if (jiggleTimer >= jiggleTimerSet && jiggle1 == true)
            {             
                jiggle1 = false;
                jiggle2 = true;
                jiggleTimer = 0;
            }
            if (jiggleTimer >= jiggleTimerSet && jiggle2 == true)
            {                
                jiggle1 = true;
                jiggle2 = false;
                jiggleTimer = 0;
            }
            
        }
        

        if (selected)
        {
            activeArrow = true;
            jiggle = true;

            
        }
        else
        {
            jiggle = false;
            transform.localScale = baseScale;
            jiggle1 = true;
            jiggle2 = false;
            jiggleTimer = 0;


        }

        if (thisItem == null)
        {
            //itemName.text = "Empty";
            ItemImage.SetActive(false);
        }
        else
        {
            ItemImage.SetActive(true);
            //itemName.text = thisItem.Name;
        }

        if (activeArrow)
        {
            foreach (GameObject arrows in arrowpoint)
            {
                arrows.SetActive(true);
            }
        }
        else
        {
            foreach (GameObject arrows in arrowpoint)
            {
                arrows.SetActive(false);
            }
        }

    }

    public void UpdateItemBody()
    {
        if ((PlayerItems.Count - 1) >= thisItemRef)
        {
            thisItem = PlayerItems[thisItemRef];

        }
        /*else if (thisItemRef < PlayerItems.Count)
        {
            itemName.text = "Empty";
        }*/
        else
        {
            
            //itemName.text = "Empty";
        }
    }   
}
