using CopiumEngine;

public class CSharpTesting : CopiumScript
{
    public int hello = 10;
    public int HI = 10;
    public string hewwo = "HELLO";

    public CSharpTesting()
    {
        Debug.Log();
        //CppFunc();
    }

    public void PrintMesssage()
    {
        //Console.WriteLine("Hello World from C");
    }

    public void PrintInts(int message, int msg2)
    {
        //Console.WriteLine($"C# says: {message} {msg2}");
    }

    public void PrintCustomMessage(string message)
    {
        //Console.WriteLine($"C# says: {message}");
    }

    public void Awake()
    {
        //Console.WriteLine("HELLO FROM CSHARPTESTING");
    }

    public void Start()
    {
        //Console.WriteLine("HELLO FROM CSHARPTESTING");
    }

    public void Update()
    {

    }

    public void LateUpdate()
    {

    }

    /*
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void CppFunc();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativeLog();*/
}
