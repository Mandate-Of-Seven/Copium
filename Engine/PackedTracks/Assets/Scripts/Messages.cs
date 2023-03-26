using CopiumEngine;
using System;

public static class Messages
{
	public static string ErrorTrainMoving = "";
	public static string ErrorMainEvent = "You need to handle the event first!";

	public static class Event_Intro
    {
		public const string body = 
			"Welcome conductor, I am AI your digital assistance for this mission. " +
			"I have a message from Captain Bob.\n\n\"Good day volunteer, \n\nYou are likely here due " +
			"to the large reward offered by the Council should you complete this mission. Although tempting, " +
			"I can't say I'd like to be in your position. The risk is just not worth it in my opinion.\n\n" +
			"Your task is to reach the settlement at the end of this railway. Recently we have lost contact " +
			"with them, and with no way to reestablish communications remotely, we are thus sending you and your crew " +
			"there to investigate. As defending the city is a priority, we are unable to send the military. " +
			"\n\nYou might have heard about the 'Crops Curse' that has been happening. Thus, we are only " +
			"able to provide your crew a small amount of rations. I'm sure you will figure something out whilst " +
			"traveling there. \n\nI wish you all the best and hope to receive good news.\n\nBest " +
			"regards, \nCaptain Bob Jones\"\n\nEnd of transmission.";
	}
	public static class Event_Explosion
    {
		public const string preempt = "There is a faint smell of something burning in the air...";
		public static class HarrisDead 
        {
			public const string body = 
				"In the dead of night, an explosion rocked the main engine room, causing massive flames " +
				"to spread to the adjacent supply storage area. The raging fire that quickly engulfed the train roused the crew.";
			public const string choice01 = "Attempt to put out the fire";
			public const string choice02 = "Detach the main engine room and supply storage room from the train";
			public const string result01 = 
				"The undermanned crew did their best to battle the blaze, but ultimately, " +
				"their efforts were futile. The flames were extinguished, but only at great personal risk. It seems " +
				"that most of the crew have suffered third to fourth degree burns.";
			public const string result02 =
				"In an effort to save themselves, the crew slashed away sections of the flaming " +
				"train as the fire raged rapidly and inexplicably. Thanks to a backup power source, the train was able " +
				"to keep going.";
		}
		public static class HarrisAlive
		{
			public const string body = 
				"Suddenly, an explosion could be heard coming from the direction of the engine room. " +
				"Massive flames enveloped the number one engine room. The speed with which the fire spread meant that it may " +
				"already have reached the supply storage area. Harris' cries for assistance can be heard throughout the train " +
				"as he sprints out of the engine room. The crew watched in horror as both Harris and their supplies caught fire.";
			public const string choice01 = "Salvage and save remaining supplies";
			public const string choice02 = "Save Harris";
			public const string result01 = 
				"The crew ignored Harris' cries for help as they focused on putting out the fire on " +
				"the supplies. By the time the crew put out and salvaged the remaining supplies, all that remained " +
				"of Harris was his charred corpse, still reaching out for help. ";
			public const string result02 =
				"The crew scrambled to put out the fire on Harris. Though they succeeded in saving Harris, " +
				"most of the supplies had been reduced to ash.";
		}
	}

	public static class Event_Intruders
	{
		public const string preempt = "As the train hurtled through the snow, a metallic clang was heard, coming from the back of the train.";
		public static class ChuckDead
		{
			public const string body =
				"Without warning, 3 mysterious individuals board the train " +
				"and enter the crew cabin. They begin shooting up the place, demanding fuel and supplies. Considering " +
				"our limited supplies, acceding to their demands may not be the best course of action.";
			public const string choice01 = "Attempt to fend off the raiders";
			public const string choice02 = "Give all your supplies";
			public const string result01 =
				"This mission was a set up!\n\nWe were sent out to die!\n\nNo... NO, Please!";
			public const string result02 =
			"The crew had no choice but to give up their hard-earned supplies to the attackers who had boarded their train." +
			"They watched in disgust as the raiders took what they wanted, humiliating the crew in the process.\n\n" +
			"The attackers sneered and laughed as they took everything they could carry, treating the crew with contempt and disdain." +
			"They reveled in the crew's humiliation, taunting them and making them feel powerless.";
		}
		public static class ChuckAlive
		{
			public const string body =
				"A mysterious vehicle was seen approaching the train, 3 mysterious " +
				"individuals proceed to board through the crew cabin. Chuck is taken by surprise and " +
				"subsequently taken hostage by the raiders. The raiders demand that we hand over our supplies, or" +
				"Chuck will be killed.";
			public const string choice01 = "Attempt to save Chuck";
			public const string choice02 = "Take out the raiders";
			public const string choice03 = "Give up supplies";
			public const string result01 =
			"Bronson knew that he had to act fast if he wanted to save his friend Chuck. Despite the risks, he charged towards the attackers, determined to take them by surprise.\n\n" +
			"As he rounded the corner, Bronson felt a sharp pain in his gut.He had been shot, but he refused to let that stop him. He pushed forward, tackling one of the attackers to the ground.\n\n" +
			"The other attackers turned their guns on him, but Bronson didn't care. He was focused on buying enough time for the rest of the crew to arrive and rescue Chuck. He fought with all his might, using his strength to overpower the attackers.\n\n" +
			"Finally, the rest of the crew arrived armed and subdued the attackers. Chuck was safe, and Bronson had proven himself to be a true hero.";
			public const string result02 =
			"As the crew prepared to retaliate against the attackers who had taken their friend Chuck hostage, they were met with a horrific sight. The attackers, sensing the crew's determination, coldly shot Chuck in the head, his body falling lifeless to the ground.\n\n" +
			"The crew was left reeling, their hearts shattered by the brutal and sudden loss of their friend. In a violent shootout, they battled the attackers with all their might, their fury fueling them even as the sound of gunfire filled the air and blood stained the ground.\n\n" +
			"In the end, the crew emerged victorious, but the cost of victory was steep.";
			public const string result03 =
			"The crew had no choice but to give up their hard-earned supplies to the attackers who had boarded their train." +
			"They watched in disgust as the raiders took what they wanted, humiliating the crew in the process.\n\n" +
			"The attackers sneered and laughed as they took everything they could carry, treating the crew with contempt and disdain."+
			"They reveled in the crew's humiliation, taunting them and making them feel powerless.";
		}
	}

	public static class Ending_DantonMissing
    {
		public const string body = "There were many days, if not weeks, with no word about Danton's whereabouts. Bronson, " +
                                         "Chuck, and Harris have all died from their wounds and the cold; I have run out of everything " +
                                         "I need to survive, including food, fuel, and water; and I'm afraid I won't be able to return " +
                                         "home alive this time. I wish you were still around, mom";
    }
	public static class Event_Bomb
	{
		public static string preempt = "As the train hurtled through the snow, a metallic clang was heard, coming from the back of the train.";
		public static class ChuckHealthy
		{
			public const string body =
				"While on routine patrol, Chuck heard a faint beeping coming from engine room 2. " +
				"He discovered a bomb concealed under the backup engine's reactor. It seems that " +
				"removing it will be challenging, as doing so may set it off. Chuck called the" +
				"remaining crew members to discuss next steps.";
			public const string choice01 = "Cover the bomb with sandbags to reduce damage";
			public const string choice02 = "Attempt to defuse the bomb, let Chuck do it";
			public const string choice03 = "Salvage parts from the back up engine to build a makeshift reactor and cut off engine room 2 from the train [requires Harris to be alive]";
			public const string result01 =
				"To minimise the damage the bomb will cause, the crew use anything they " +
				"can find to cover it. As they worked, the beeping on the bomb stopped and it exploded. " +
				"As the dust settled, Chuck and Danton can be seen on the ground, it is evident that both are in critical condition." +
				"Fortunately, the engine only suffered minor damage.";
			public const string result02 =
				"Chuck attempted to defuse the bomb, but upon cutting a red wire, the timer started ticking down faster. " +
				"Seeing no other alternative, Chuck ripped the bomb out and quickly tossed it out " +
				"of the train cart. Although Chuck saved the engine, Chuck was struck in the head by sharpnel " +
				"killing him in an instant.";

			public const string result03 =
				"Using whatever spare parts were available, Harris managed to keep the train running with his makeshift reactor." +
				" The crew continue on their mission but there is no longer any supplies left on the train.";
		}
		public static class CrewInjured
		{
			public const string body =
				"While attempting to keep Bronson, Chuck and Harris alive, another explosion " +
				"went off in the depths of the train. Danton could not tell what was going on but the " +
				"train came grinding to a halt.";
			public const string choice01 = "Send Danton out to find supplies";
			public const string choice02 = "Take out the raiders";
			public const string choice03 = "Give up supplies";
			public const string result01 =
			"Bronson knew that he had to act fast if he wanted to save his friend Chuck. Despite the risks, he charged towards the attackers, determined to take them by surprise.\n\n" +
			"As he rounded the corner, Bronson felt a sharp pain in his gut.He had been shot, but he refused to let that stop him. He pushed forward, tackling one of the attackers to the ground.\n\n" +
			"The other attackers turned their guns on him, but Bronson didn't care. He was focused on buying enough time for the rest of the crew to arrive and rescue Chuck. He fought with all his might, using his strength to overpower the attackers.\n\n" +
			"Finally, the rest of the crew arrived armed and subdued the attackers. Chuck was safe, and Bronson had proven himself to be a true hero.";
			public const string result02 =
			"As the crew prepared to retaliate against the attackers who had taken their friend Chuck hostage, they were met with a horrific sight. The attackers, sensing the crew's determination, coldly shot Chuck in the head, his body falling lifeless to the ground.\n\n" +
			"The crew was left reeling, their hearts shattered by the brutal and sudden loss of their friend. In a violent shootout, they battled the attackers with all their might, their fury fueling them even as the sound of gunfire filled the air and blood stained the ground.\n\n" +
			"In the end, the crew emerged victorious, but the cost of victory was steep.";
			public const string result03 =
			"The crew had no choice but to give up their hard-earned supplies to the attackers who had boarded their train." +
			"They watched in disgust as the raiders took what they wanted, humiliating the crew in the process.\n\n" +
			"The attackers sneered and laughed as they took everything they could carry, treating the crew with contempt and disdain." +
			"They reveled in the crew's humiliation, taunting them and making them feel powerless.";
		}

		//Someone dead, someone other than danton that is not critically injured
		public static class Default
        {
			public const string body =
				"Out of nowhere an explosion ripped through the back up engine room, quickly engulfing " +
				"the back of the train in flames";
			public const string choice01 = "Watch the flames destroy the engine";
			public const string choice02 = "Put out the fire and attempt to save the engine";
			public const string choice03 = "Build new simple engine to power the train";
			public const string result01 =
				"As you and your crew watch the engine burst into flames, the train starts slowing down. " +
				"Lights in the train start dimming out... \n\nThe train becomes silent...";
			public const string result02 =
				"The crew scramble to put out the fire before it could spread any further. It seems " +
				"that the engine can continue running, but the crew suffered minor burns.";
			public const string result03 =
				"To assemble the makeshift reactor, most of the crew's remaining supplies have been exhausted." +
				" It serves its purpose, but you notice that the train has started moving much slower.";
		}
	}
}
