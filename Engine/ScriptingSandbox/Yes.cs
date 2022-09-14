using System;
using System.Runtime.CompilerServices;

public class Yes
{
    public int hello = 10;
    public int HI = 10;
    public string hewwo = "HELLO";

    public Yes()
    {
        Console.WriteLine("Hello matthew");
        CppFunc();
    }

    public void PrintMessage()
    {
        Console.WriteLine("Hello World from C#");
    }

    public void PrintInts(int message, int msg2)
    {
        Console.WriteLine($"C# says: {message} {msg2}");
    }

    public void PrintCustomMessage(string message)
    {
        Console.WriteLine($"C# says: {message}");
    }

    public void Awake()
    {
        Console.WriteLine("HELLO FROM CSHARPTESTING");
    }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static void CppFunc();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static void NativeLog();
}
