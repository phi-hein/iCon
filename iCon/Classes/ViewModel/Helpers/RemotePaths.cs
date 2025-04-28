using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace iCon_General
{
    /// <summary>
    /// Utility class for handling paths that are separated by forward-slashes
    /// </summary>
    internal static class RemotePaths
    {
        /// <summary>
        /// Check if a remote path is an absolute path
        /// </summary>
        public static bool IsPathRooted(string i_path)
        {
            if (string.IsNullOrWhiteSpace(i_path) == true)
            {
                return false;
            }

            string t_path = i_path.Trim();
            if (t_path[0] == '/')
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Removes slash(es) at the end of a path
        /// </summary>
        public static string RemoveEndSlash(string i_path)
        {
            if (string.IsNullOrWhiteSpace(i_path) == true)
            {
                return "";
            }

            string t_path = i_path.Trim();
            while ((t_path.Length > 0) && (t_path[t_path.Length - 1] == '/'))
            {
                t_path = t_path.Remove(t_path.Length - 1);
            }

            return t_path;
        }

        /// <summary>
        /// Combine two paths 
        /// (if the second one is an absolute path, then this returns the second path),
        /// the result has no directory delimiter at the end
        /// </summary>
        public static string Combine(string i_path1, string i_path2)
        {
            string t_path1 = RemoveEndSlash(i_path1);
            string t_path2 = RemoveEndSlash(i_path2);

            if (string.IsNullOrWhiteSpace(t_path1) == true)
            {
                return t_path2;
            }

            if (string.IsNullOrWhiteSpace(i_path2) == true)
            {
                return t_path1;
            }

            if (IsPathRooted(t_path2) == true)
            {
                return t_path2;
            }

            return t_path1 + "/" + t_path2;
        }

        /// <summary>
        /// Combine three paths 
        /// (if the third one is an absolute path, then this returns the third path),
        /// (if the second one is an absolute path, then this returns the combination of second and third path),
        /// the result has no directory delimiter at the end 
        /// </summary>
        public static string Combine(string i_path1, string i_path2, string i_path3)
        {
            return Combine(i_path1, Combine(i_path2, i_path3));
        }

        /// <summary>
        /// Create a list of all directories that are contained in a path
        /// </summary>
        public static List<string> GetDirectoryList(string i_path)
        {
            string t_path = i_path.Trim();
            bool is_absolute = IsPathRooted(t_path);

            char[] delim = new char[] { '/' };
            string[] dirs = t_path.Split(delim, StringSplitOptions.RemoveEmptyEntries);

            List<string> t_result = new List<string>();
            if (dirs.Length != 0)
            {
                string t_concatstr = "";
                if (is_absolute == true)
                {
                    t_concatstr += "/";
                }
                for (int i = 0; i < dirs.Length; i++)
                {
                    if (string.IsNullOrWhiteSpace(dirs[i]) == true) continue;
                    string t_adddir = t_concatstr + dirs[i];
                    t_result.Add(t_adddir);
                    t_concatstr += dirs[i] + "/";
                }
            }

            return t_result;
        }
    }
}
