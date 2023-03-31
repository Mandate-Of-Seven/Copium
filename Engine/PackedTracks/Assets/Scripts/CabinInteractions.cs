using CopiumEngine;
using System;
public class CabinInteractions: CopiumScript
{

	public GameManager gm;

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

	public int dialoguePartnerIndex = -1;

	public Animator dialogueSprite;

	public Text dialogueText;
	public StringTypeWriterEffect writer;
	public float writerInterval = 0.7f;
	HarrisDialogue hd = new HarrisDialogue();
	BronsonDialogue bd = new BronsonDialogue();
	ChuckDialogue cd = new ChuckDialogue();
	DantonDialogue dd = new DantonDialogue();

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

			// Type writer
			if(writer != null)
				dialogueText.text = writer.Write();


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

		// Display dialogue
		switch(crewIdx)
		{
			case 0:
			{
				DisplayDialogue(gm.eventSequence, hd, crewIdx);
				break;
			}
			case 1:
			{
				DisplayDialogue(gm.eventSequence, bd, crewIdx);
				break;
			}
			case 2:
			{
				DisplayDialogue(gm.eventSequence, cd, crewIdx);
				break;
			}
			case 3:
			{
				DisplayDialogue(gm.eventSequence, dd, crewIdx);
				break;
			}
			default:
			{
				writer = new StringTypeWriterEffect("Khajiit has wares, if you have coin to spare....\nOops, wrong franchise...I'll take my leave", writerInterval);
				break;
			}
		}


	}
	public void CloseDialogueBox(){
		crewMenu.interacting = false;
		dialogueBox.SetActive(false);
		CloseDialogueBtnWrapper.SetInteractable(false);

		SetInteractBtnsInteractability(true);
		SetInteractBtnsActive(true);

	}
	public void SetInteractBtnsInteractability(bool _flag){
		if(crewMenu.harris.person.alive)
			harrisInteractBtnWrapper.SetInteractable(_flag);

		if(crewMenu.bronson.person.alive)
			bronsonInteractBtnWrapper.SetInteractable(_flag);

		if(crewMenu.chuck.person.alive)
			chuckInteractBtnWrapper.SetInteractable(_flag);

		if(crewMenu.danton.person.alive)
			dantonInteractBtnWrapper.SetInteractable(_flag);
	}
	public void SetInteractBtnsActive(bool _flag){
		if(crewMenu.harris.person.alive)
			harrisBtn.SetActive(_flag);

		if(crewMenu.bronson.person.alive)
			bronsonBtn.SetActive(_flag);

		if(crewMenu.chuck.person.alive)
			chuckBtn.SetActive(_flag);

		if(crewMenu.danton.person.alive)
			dantonBtn.SetActive(_flag);
	}

	int CheckLowStats(int _crewIdx)
	{
		if(_crewIdx > 3 || _crewIdx < 0)
			return -1;

		string _name = null;
		switch(_crewIdx)
		{
			case 0:
			{
				_name = "Harris";
				break;
			}
			case 1:
			{
				_name = "Bronson";
				break;
			}
			case 2:
			{
				_name = "Chuck";
				break;
			}
			case 4:
			{
				_name = "Danton";
				break;
			}
		}

		if(_name == null)
			return -1;
		
		if(crewMenu.crewMembers[_name].health == HEALTH_STATE.CRITICAL)
			return 1;

		if(crewMenu.crewMembers[_name].hunger == HUNGER_STATE.FAMISHED)
			return 2;

		if(crewMenu.crewMembers[_name].mental == MENTAL_STATE.SUICIDAL)
			return 3;

		return 0;
	}


#region DIALOGUE

// first check event
// second check stats
// third  
	public class DialogueSet
	{
		public string[] Lines = new string[3]
		{
			"null", 
			"null", 
			"null"
		};
		public int count = 3;

		public string RNGLine()
		{

			if(count > 0)
			{
				int i = RNG.Range(0, count - 1);
				return Lines[i];
			}

			return "null";
		}

		public string GetLine(int idx)
		{
			if(idx < 0 || idx >= count)
				return System.String.Empty;

			return Lines[idx];
		}
	}

	public abstract class Dialogue
	{
		public virtual string GetFirst()
		{
			return first.RNGLine();
		}
		public virtual string GetSecond()
		{
			return second.RNGLine();
		}
		public virtual string GetThird()
		{
			return third.RNGLine();
		}
		public virtual string Ending()
		{
			return ending.RNGLine();
		}
		public virtual string GetLow(int i)
		{
			if(i >= 0 && i < 3)
				return low.Lines[i];

			return low.RNGLine();
		}
		public virtual string EasterEgg()
		{
			return "Khajiit has wares, if you have coin to spare....\nOops, wrong franchise...I'll take my leave";
		}


		public DialogueSet first = new DialogueSet();
		public DialogueSet second = new DialogueSet();
		public DialogueSet third = new DialogueSet();
		public DialogueSet ending = new DialogueSet();
		public DialogueSet low = new DialogueSet();
		public DialogueSet ee = new DialogueSet();


		public int idx;
	}

	public class HarrisDialogue : Dialogue
	{
		public HarrisDialogue()
		{

			Console.WriteLine("harris dialogue ctor");


			#region First
			first.Lines[0] = "Hey Boss, the train engine is fascinating, the backup engine is simple but the main engine... whoever built it, I hope he's still around somewhere.";
			first.Lines[1] = "Chuck and Bronson seem to be having a little debate over some personal issues, maybe you should check on them.";
			first.Lines[2] = "They didn't give us much rations, but it should last us for a little while.";
			first.count = 3;
			#endregion
			#region Second
			second.Lines[0] = "Hey Boss, we're running low on supplies. We should stop the train and salvage anything nearby.";
			second.Lines[1] = "Yo Boss that fire got me good huh. Thanks for saving me. I gotta tell you Boss, that fire was no accident. Someone did something to the engine, trust me I can tell.";
			second.Lines[2] = "Boss, we haven't heard from that city in four months, don't you think the city is in some kind of trouble? ";
			second.count = 3;
			#endregion
			#region Third
			third.Lines[0] = "Good thing those tracks are still intact, I doubt we'd be able to repair em with the equipment on hand.";
			third.Lines[1] = "Man it's getting colder in here, the heating must be breaking down. I'll go take a look at it later.";
			third.count = 2;
			#endregion
			#region Ending
			ending.Lines[0] = "Urghhh, it's so cold... I gotta... get warm somehow...";
			ending.count = 1;
			#endregion
			#region Low
			low.Lines[0] = "Hey Boss, these injuries ain't too bad. Lemme know if ya need anything.";
			low.Lines[1] = "Boss, we gotta go out and find some supplies.";
			low.Lines[2] = "Heh, Hehehe... HAHAHAHAHA!";
			low.count = 3;
			#endregion
			#region EE
			ee.count = 0;
			#endregion


			idx = 0;
		}
	}

	public class BronsonDialogue : Dialogue
	{
		public BronsonDialogue()
		{

			Console.WriteLine("bronson dialogue ctor");


			#region First
			first.Lines[0] = "Harris, he... I couldn't. He was begging for help. I think I need some time to myself.";
			first.Lines[1] = "I don't trust Chuck. He said that if it comes down to it, he would ensure his safety first instead of helping those in need.";
			first.Lines[2] = "I hope we all get to the other city safely.";
			first.count = 3;
			#endregion
			#region Second
			second.Lines[0] = "Thank goodness we have someone like Danton. He's been helping me with these small injuries. Truth be told, I'm not too sure what I'd do without him";
			second.Lines[1] = "How did it happen? Why... Sorry Cap, I think I need some time to process what just happened.";
			second.Lines[2] = "All my strength and yet I was powerless against the fire...";
			second.count = 3;
			#endregion
			#region Third
			third.Lines[0] = "What do you think happened to the train that went to the city two months ago? Rumor has it that the train was lot and the crew were presumed dead.";
			third.Lines[1] = "Did you hear what Chuck said? He said we should torture then kill the raiders. The world is already bad as it is why the unnecessary violence?";
			third.count = 2;
			#endregion
			#region Ending
			ending.Lines[0] = "I never... I never wanted to kill him. It wasn't my fault... Please forgive me...";
			ending.Lines[1] = "Hey Cap, do you think I'm a monster for killing the man that tried to raid us? Well, I know the rest do... Please leave me be for abit.";
			ending.count = 2;
			#endregion
			#region Low
			low.Lines[0] = "Hey Cap, sorry but I'm not in the best of shape... Please let me rest.";
			low.Lines[1] = "Let's find some supplies Cap, I'm starting to get real hungry.";
			low.Lines[2] = "GET AWAY FROM ME!";
			low.count = 3;
			#endregion
			#region EE
			ee.Lines[0] = "Pain is just weakness leaving the body";
			ee.count = 1;
			#endregion


			idx = 1;
		}
	}

	public class ChuckDialogue : Dialogue
	{
		public ChuckDialogue()
		{

			Console.WriteLine("chuck dialogue ctor");


			#region First
			first.Lines[0] = "Hey Cap'n, do you know anything about Captain Bob? Seems like he just joined the city's defence force as a new officer not too long ago. Wonder how he got up to that position so fast?";
			first.Lines[1] = "If you ask me Cap'n, Bronson can't be trusted. A soft man like him is simply not suited for such a mission, perhaps we should toss him out of the train...";
			first.Lines[2] = "Have you ever wondered how the world ended up like this? Based on my research, it seems the clouds above our heads are manmade...";
			first.count = 3;
			#endregion
			#region Second
			second.Lines[0] = "Uhhh Cap'n, just out of curiosity, is there anything else to eat?";
			second.Lines[1] = "Hmm... It doesn't seem like Harris was behind the explosion. I'm guessing someone else may have planted a bomb of some kind.";
			second.Lines[2] = "How could this have happened? Perhaps I should conduct my own investigation into this...";
			second.count = 3;
			#endregion
			#region Third
			third.Lines[0] = "I heard the last train that went to the city got lost, let's hope we don't end up like them.";
			third.Lines[1] = "Hey Cap'n, those damn raiders, they really have no mercy. if I had the chance, I'd make sure they die a horrible death.";
			third.Lines[2] = "My stomach... Urghhh, they got me good Cap'n.";
			third.count = 3;
			#endregion
			#region Ending
			ending.Lines[0] = "Cap'n, heres my theory, the city we are going to, it was taken over and our city is going to be taken over soon too.";
			ending.Lines[1] = "I wonder, does the council ever change its members?";
			ending.Lines[2] = "We must not go to the city, I think we should stop.";
			ending.count = 3;
			#endregion
			#region Low
			low.Lines[0] = "Ah Cap'n, I'm ok, I just need some rest.";
			low.Lines[1] = "Cap'n, supplies are low. Lets get out there and find some supplies, I'm getting hungry.";
			low.Lines[2] = "Shut up, shut up, SHUT UP!";
			low.count = 3;
			#endregion
			#region EE
			ee.Lines[0] = "Are we there yet?";
			ee.count = 1;
			#endregion


			idx = 2;
		}
	}

	public class DantonDialogue : Dialogue
	{
		public DantonDialogue()
		{

			Console.WriteLine("danton dialogue ctor");


			#region First
			first.Lines[0] = "This train does not seem safe to me.";
			first.Lines[1] = "I can't believe all it took was one mistake for them to exile me to this place";
			first.Lines[2] = "The snow... do you think it will end one day?";
			first.count = 3;
			#endregion
			#region Second
			second.Lines[0] = "If there is one thing I realised, is that all it takes is one small mistake for others to start looking at you differently.";
			second.Lines[1] = "I knew this train was dangerous... how did that fire get out of hand that quickly?";
			second.Lines[2] = "I know we have to complete this mission. But at what cost?";
			second.count = 3;
			#endregion
			#region Third
			third.Lines[0] = "Wow, they really did a number on the train huh... humanity really has degraded";
			third.Lines[1] = "I think I recognised one of the raiders, I'm not a hundred percent certain but I think he came into my clinic complaining about a skin infection a few days before I was sent here.";
			third.Lines[2] = "Don't you think its funny how with enough people protesting and rioting you can make the council do things they wouldn't normally do.";
			third.count = 3;
			#endregion
			#region Ending
			ending.Lines[0] = "Hey Sir, these past few days have taken a toll on everyone. You might want to check up on Bronson, he looks a little worse for wear.";
			ending.Lines[1] = "Sir, there's only so much I can do. Without the right medical equipment, the only thing I can do now is to help relieve the pain.";
			ending.count = 2;
			#endregion
			#region Low
			low.Lines[0] = "I'm...doing fine, Sir... the priority is the rest of the crew...";
			low.Lines[1] = "Sir, we're running dangerously low on supplies. We're going to need more if we want to survive.";
			low.Lines[2] = "No, its not me, its not me, its not me....";
			low.count = 3;
			#endregion
			#region EE
			ee.Lines[0] = "Puh puh puh puh, Pills over here!";
			ee.count = 1;
			#endregion


			idx = 2;
		}
	}

	public void DisplayDialogue(int eventIdx, Dialogue d, int _crewIdx)
	{

		// Check for low stats first
		int tmp = CheckLowStats(_crewIdx);

		if(tmp <= 0)
		{
			int e = RNG.Range(0, 4);
			if(e == 0)
			{
				writer = new StringTypeWriterEffect(d.EasterEgg(), writerInterval);
				return;
			}

			switch(eventIdx)
			{
				case 0:
				{
					writer = new StringTypeWriterEffect(d.GetFirst(), writerInterval);
					break;
				}
				case 1:
				{
					writer = new StringTypeWriterEffect(d.GetSecond(), writerInterval);
					break;
				}
				case 2:
				{
					writer = new StringTypeWriterEffect(d.GetThird(), writerInterval);
					break;
				}
				case 3:
				{
					writer = new StringTypeWriterEffect(d.Ending(), writerInterval);
					break;
				}
			}			
		}else
		{
			int e = RNG.Range(0,2);
			if((e % 2) == 0)
			{
				writer = new StringTypeWriterEffect(d.GetLow(tmp), writerInterval);
			}else
			{
				switch(eventIdx)
				{
					case 0:
					{
						writer = new StringTypeWriterEffect(d.GetFirst(), writerInterval);
						break;
					}
					case 1:
					{
						writer = new StringTypeWriterEffect(d.GetSecond(), writerInterval);
						break;
					}
					case 2:
					{
						writer = new StringTypeWriterEffect(d.GetThird(), writerInterval);
						break;
					}
					case 3:
					{
						writer = new StringTypeWriterEffect(d.Ending(), writerInterval);
						break;
					}
				}
			}
		}


	}

#endregion

}