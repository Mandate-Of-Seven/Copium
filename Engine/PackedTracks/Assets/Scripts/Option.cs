using CopiumEngine;
using System;

public class Option: CopiumScript
{
    public Text txt;
    public Button btn;

	public GameObject icon_01, icon_02, icon_03, icon_04;

    void Start()
	{
		icon_01.SetActive(false);
        icon_02.SetActive(false);
        icon_03.SetActive(false);
        icon_04.SetActive(false);
	}
	void Update()
	{

	}

    public void Enable()
    {
        if(!gameObject.activeSelf)
            gameObject.SetActive(true);
    }

    public void Disable()
    {
        if(gameObject.activeSelf)
            gameObject.SetActive(false);
    }

    public void ResetOption()
    {
        Disable();
        HideAllIcons();
        btn.enabled = true;
    }

	public void ShowAllIcons()
	{
        icon_01.SetActive(true);
        icon_02.SetActive(true);
        icon_03.SetActive(true);
        icon_04.SetActive(true);
    }

    public void ShowIcons(bool health = false, bool mental = false, bool hunger = false, bool supplies = false)
    {
        if(health)
            icon_01.SetActive(true);

        if (mental)
            icon_02.SetActive(true);

        if (hunger)
            icon_03.SetActive(true);

        if (supplies)
            icon_04.SetActive(true);
    }

    public void HideAllIcons()
    {
        icon_01.SetActive(false);
        icon_02.SetActive(false);
        icon_03.SetActive(false);
        icon_04.SetActive(false);
    }

    public void HideIcons(bool health = false, bool mental = false, bool hunger = false, bool supplies = false)
    {
        if (health)
            icon_01.SetActive(false);

        if (mental)
            icon_02.SetActive(false);

        if (hunger)
            icon_03.SetActive(false);

        if (supplies)
            icon_04.SetActive(false);
    }
}
