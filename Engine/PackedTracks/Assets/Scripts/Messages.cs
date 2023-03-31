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
				"The darkness of the night was shattered by a deafening blast that ripped through the heart of the train, unleashing a devastating explosion that shook everything to its core. " +
                "In an instant, chaos erupted as flames erupted from the Main engine room and spread with ruthless efficiency towards the food storage area.The fire blazed with a fury that defied all attempts to contain it, growing stronger and fiercer by the second. " +
                "\n\nThe crew were jolted from their slumber, thrown into a nightmare scenario that seemed to defy all logic and reason. " +
                "The situation was nothing short of shocking, a hellish inferno that threatened to consume everything in its path with merciless efficiency.";
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
				"Suddenly, a thunderous explosion ripped through the silence, jolting the train off its tracks. " +
                "The ground shook beneath their feet, thick black smoke billowed through the carriages, choking their lungs and blinding their vision.\n\n" +
                "The crew members bolted towards the source of the commotion, their hearts pounding with adrenaline and fear.As they reached the engine room, they were met with a scene straight out of hell. " +
                "Flames leapt from every corner of the room, devouring everything in their path with an insatiable hunger.\n\n" +
                "The fire was spreading at a breakneck speed, already engulfing the food storage area in its fiery embrace.The team knew they had little time to act before the entire train was consumed by the inferno.\n\n" +
                "Harris's panicked cries for help rang through the air, but they were barely audible over the roar of the flames. " +
                "The crew watched in horror as he emerged from the engine room, his body ablaze with flames.";
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
				"The sound of gunshots shattered the silence on the train as three masked and armed figures appeared out of nowhere.The trio stormed towards the cabin, their guns trained on anyone who dared to move.\n\n" +
                "The leader of the group barked orders at the crew, demanding fuel and supplies, threatening to kill them all if they didn't comply. " +
                "But the crew knew that giving away more supplies could endanger everyone on board, leaving them stranded in the middle of nowhere with nothing to sustain them.\n\n" +
                "As tensions rose and the situation became more dangerous by the minute, the crew knew they had to act fast.";
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
				"As the train hurtled down the tracks, a shadowy figure appeared on the horizon, a vehicle unlike any they had ever seen. It screeched to a halt, and three masked figures emerged, storming towards the train with a deadly purpose.\n\n" +
                "The crew members were caught off-guard, their nerves on edge as the intruders boarded the train via the crew compartment. " +
                "Chuck, one of the crew members, found himself face-to-face with the attackers, his heart pounding with fear as he realized he was their captive.\n\n" +
                "The leader of the group held a gun to Chuck's head, demanding that the crew provide them with food and fuel, or suffer the consequences. " +
                "The situation was dire, and the crew knew that they had to act fast if they wanted to survive.";
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

	public static class Event_Bomb
	{
		public static string preempt = "As the train hurtled through the snow, a metallic clang was heard, coming from the back of the train.";
		public static class ChuckHealthy
		{
			public const string body =
				"While on routine patrol, Chuck heard a faint, steady bleeping emanating from engine room 2. " +
                "Chuck discovered a bomb concealed under the backup engine's reactor after looking for it there. " +
                "It seems that removing it will be challenging, since doing so might trigger the explosion. " +
                "Chuck called a meeting of the remaining crew members to discuss next steps.\n\n" +
                "As Chuck made his way through the train on his routine patrol, he heard a faint, steady bleeping sound emanating from the engine room. " +
                "His heart raced as he realized what it could mean. He rushed to the engine room and quickly located the source of the sound - a bomb, hidden expertly beneath the backup engine's reactor.\n\n" +
                "The shock of the discovery was overwhelming.The crew members knew that if the bomb detonated, it could mean the end of everything they held dear. " +
                "But they also knew that removing it would be a delicate and dangerous task, one that could trigger the explosion and cost them their lives.\n\n" +
                "As Chuck called an emergency meeting of the remaining crew members to discuss next steps, the tension in the room was palpable. They knew that they had to act fast, but they also knew that the slightest mistake could have catastrophic consequences.";
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
				"As Danton scrambled to keep Bronson, Chuck, and Harris stable, the air was suddenly torn apart by a deafening explosion. " +
                "The force of the blast rocked the train, sending it careening off the tracks and into the darkness beyond.\n\n" +
                "Danton struggled to maintain his balance as the train lurched and shook, his heart racing with fear and adrenaline. " +
                "He could hear the panicked cries of his fellow crew members as they desperately tried to keep the train from derailing completely.\n\n" +
                "But despite their best efforts, the train ground to a halt, shuddering to a stop in the middle of nowhere. " +
                "The darkness outside was absolute, and the silence that followed the explosion was deafening.\n\n" +
                "Danton knew that something was horribly wrong.The train was dead in the water, with no power and no means of communication. " +
                "He could feel the weight of the situation bearing down on him, as the reality of their predicament sunk in.";

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
				"Without warning, a thunderous explosion erupted from the backup engine room, unleashing a raging inferno that engulfed the entire back section of the train. " +
                "The flames roared and crackled with an insatiable hunger, mercilessly consuming everything in its path.\n\n" +
                "Within seconds, the lights started flickering and the train began slowing down. " +
                "The crew must think quickly, or they will perish.";
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

    public static class Ending_3B
    {
        public const string body = "There were many days, if not weeks, with no word about Danton's whereabouts. Bronson, " +
                                         "Chuck, and Harris have all died from their wounds and the cold; I have run out of everything " +
                                         "I need to survive, including food, fuel, and water; and I'm afraid I won't be able to return " +
                                         "home alive this time. I wish you were still around, mom";
    }

    public static class Ending_2A
    {
        public const string body = "A trap was prepared for us on this expedition! We were all sent out to die. No... No!";
    }
}
