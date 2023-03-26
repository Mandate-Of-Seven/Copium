using CopiumEngine;
using System;

public class InteractionMask: CopiumScript
{
	public static InteractionMask Instance;

	public GameObject rightPivot;
	public GameObject leftPivot;
	public GameObject topPivot;
	public GameObject bottomPivot;


	public Vector2 scale = new Vector2(1,1);

	void Awake()
    {
		Instance = this;
    }

	void Start()
	{

	}

	void Update()
	{
		Vector3 rightPos = new Vector3(scale.x / 2f, 0f, 0f);
		Vector3 leftPos = new Vector3(-scale.x / 2f, 0f, 0f);
		Vector3 topPos = new Vector3(0, scale.y/2f, 0f);
		Vector3 bottomPos = new Vector3(0, -scale.y / 2f, 0f);
		Vector3 horizontalScale = new Vector3(scale.x, 1, 1);

		rightPivot.transform.position = rightPos;
		leftPivot.transform.position = leftPos;
		topPivot.transform.position = topPos;
		bottomPivot.transform.position = bottomPos;
		topPivot.transform.localScale = horizontalScale;
		bottomPivot.transform.localScale = horizontalScale;
	}
}
