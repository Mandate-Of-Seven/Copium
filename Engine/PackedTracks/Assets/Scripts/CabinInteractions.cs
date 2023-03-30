using CopiumEngine;
using System;
public class CabinInteractions: CopiumScript
{
	public CrewMenu crewMenu;
	public GameObject harrisIText;
	public GameObject bronsonIText;
	public GameObject chuckIText;
	public GameObject dantonIText;


	public GameObject harrisBtn;
	public Button harrisInteractBtn;
	ButtonWrapper harrisInteractBtnWrapper;

	public GameObject bronsonBtn;
	public Button bronsonInteractBtn;
	ButtonWrapper bronsonInteractBtnWrapper;

	public GameObject chuckBtn;
	public Button chuckInteractBtn;
	ButtonWrapper chuckInteractBtnWrapper;

	public GameObject dantonBtn;
	public Button dantonInteractBtn;
	ButtonWrapper dantonInteractBtnWrapper;
	
	public bool hovering = false;
	public bool[] hover = new bool[4];	// Keep track of which crew member is being hovered over, there should only be one bool true at any time

	public GameObject dialogueBox;
	public Button CloseDialogueBtn;
	ButtonWrapper CloseDialogueBtnWrapper;

	public Animator dialogueSprite;

	void Start()
	{		
		harrisInteractBtnWrapper = new ButtonWrapper(harrisInteractBtn);
        harrisInteractBtnWrapper.SetText(harrisInteractBtn.GetComponent<Text>());

		bronsonInteractBtnWrapper = new ButtonWrapper(bronsonInteractBtn);
        bronsonInteractBtnWrapper.SetText(bronsonInteractBtn.GetComponent<Text>());

		chuckInteractBtnWrapper = new ButtonWrapper(chuckInteractBtn);
        chuckInteractBtnWrapper.SetText(chuckInteractBtn.GetComponent<Text>());

		dantonInteractBtnWrapper = new ButtonWrapper(dantonInteractBtn);
        dantonInteractBtnWrapper.SetText(dantonInteractBtn.GetComponent<Text>());

		CloseDialogueBtnWrapper = new ButtonWrapper(CloseDialogueBtn);

		dialogueSprite.stop();
	}
	void Update()
	{
		if(!crewMenu.deploying && !crewMenu.preparing)
		{
			if(!crewMenu.interacting)
			{
				InteractButtonInputs();				
			}
			//Console.WriteLine("blooping");

		}

		if(crewMenu.interacting)
		{
			// Dialogue Logic
			DialogueButtonInputs();
		}

	}
	public void InteractButtonInputs()
	{
		// Harris
		if(harrisInteractBtnWrapper.GetState() == ButtonState.OnHover && !hover[0])
		{
			harrisIText.SetActive(true);
			hover[0] = true;
		}else if(harrisInteractBtnWrapper.GetState() != ButtonState.OnHover && hover[0]){
			harrisIText.SetActive(false);
			hover[0] = false;

		}

		if(bronsonInteractBtnWrapper.GetState() == ButtonState.OnHover && !hover[1])
		{
			bronsonIText.SetActive(true);
			hover[1] = true;
		}else if(bronsonInteractBtnWrapper.GetState() != ButtonState.OnHover && hover[1]){
			bronsonIText.SetActive(false);
			hover[1] = false;

		}

		if(chuckInteractBtnWrapper.GetState() == ButtonState.OnHover && !hover[2])
		{
			chuckIText.SetActive(true);
			hover[2] = true;
		}else if(chuckInteractBtnWrapper.GetState() != ButtonState.OnHover && hover[2]){
			chuckIText.SetActive(false);
			hover[2] = false;

		}

		if(dantonInteractBtnWrapper.GetState() == ButtonState.OnHover && !hover[3])
		{
			dantonIText.SetActive(true);
			hover[3] = true;
		}else if(dantonInteractBtnWrapper.GetState() != ButtonState.OnHover && hover[3]){
			dantonIText.SetActive(false);
			hover[3] = false;

		}
		
		// else if(crewMenu.bronson.selectBtnWrapper.GetState() == ButtonState.OnHover && hovering == false)
		// {
		// 	interactText.SetActive(true);
		// 	hovering = true;
		// }else if(crewMenu.bronson.selectBtnWrapper.GetState() != ButtonState.OnHover && hovering == true)
		// {
		// 	interactText.SetActive(false);
		// 	hovering = false;
		// }




		if(harrisInteractBtnWrapper.GetState() == ButtonState.OnClick){
			harrisIText.SetActive(false);
			hover[0] = false;

			OpenDialogueBox(0);

		}
		if(bronsonInteractBtnWrapper.GetState() == ButtonState.OnClick){
			bronsonIText.SetActive(false);
			hover[1] = false;

			OpenDialogueBox(1);

		}
		if(chuckInteractBtnWrapper.GetState() == ButtonState.OnClick){
			chuckIText.SetActive(false);
			hover[2] = false;

			OpenDialogueBox(2);

		}
		if(dantonInteractBtnWrapper.GetState() == ButtonState.OnClick){
			dantonIText.SetActive(false);
			hover[3] = false;

			OpenDialogueBox(3);

		}



	}
	public void DialogueButtonInputs(){
		if(CloseDialogueBtnWrapper.GetState() == ButtonState.OnClick){
			CloseDialogueBox();
		}
	}

	public void OpenDialogueBox(int crewIdx){
		crewMenu.interacting = true;
		dialogueBox.SetActive(true);
		CloseDialogueBtnWrapper.SetInteractable(true);

		// Disable other interact buttons
		SetInteractBtnsInteractability(false);
		SetInteractBtnsActive(false);
		dialogueSprite.setFrame(crewIdx);
	}
	public void CloseDialogueBox(){
		crewMenu.interacting = false;
		dialogueBox.SetActive(false);
		CloseDialogueBtnWrapper.SetInteractable(false);

		SetInteractBtnsInteractability(true);
		SetInteractBtnsActive(true);

	}
	public void SetInteractBtnsInteractability(bool _flag){
		harrisInteractBtnWrapper.SetInteractable(_flag);
		bronsonInteractBtnWrapper.SetInteractable(_flag);
		chuckInteractBtnWrapper.SetInteractable(_flag);
		dantonInteractBtnWrapper.SetInteractable(_flag);
	}
	public void SetInteractBtnsActive(bool _flag){
		harrisBtn.SetActive(_flag);
		bronsonBtn.SetActive(_flag);
		chuckBtn.SetActive(_flag);
		dantonBtn.SetActive(_flag);
	}

}