using CopiumEngine;
using System;

public class TutorialText: CopiumScript
{
	public static TutorialText Instance;
	StringTypeWriterEffect writer;
	Text text;
	public float interval = 0.1f;

	void Awake()
    {
		Instance = this;
    }

	void Start()
	{
		text = GetComponent<Text>();

	}

	public void SetContent(string content)
    {
		writer = new StringTypeWriterEffect(content, interval);
    }

	public bool Done()
    {
		return writer == null || writer.Done();
	}

	void Update()
	{
		if (PauseMenu.Instance.isPaused)
			return;

		if (!Done())
		{
			if (Input.GetKeyDown(KeyCode.Space))
            {
				writer.Skip();
			}
			text.text = writer.Write();
        }
	}
}
