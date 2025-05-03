namespace iCon_General
{
    /// <summary>
    /// Func with an out parameter
    /// </summary>
    /// <typeparam name="T"> Flag type </typeparam>
    /// <typeparam name="TResult"> Return type </typeparam>
    /// <returns></returns>
    public delegate TResult FlagFunc<T, out TResult>(out T flag);
}