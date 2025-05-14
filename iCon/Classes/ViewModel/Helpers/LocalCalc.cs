using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace iCon_General
{
    internal static class LocalCalc
    {
        /// <summary>
        /// Creates a local directory (and all missing directories within the path)
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="path">Directory path (absolute)</param>
        /// <returns>"true" if successful</returns>
        public static bool CreateDirectory(DoWorkEventArgs e, string path)
        {
            try
            {
                _ = Directory.CreateDirectory(path);
            }
            catch (PathTooLongException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Directory path: " + path);
                e.Result = new BWorkerResultMessage("Directory Error", "Path too long\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (DirectoryNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Directory path: " + path);
                e.Result = new BWorkerResultMessage("Directory Error", "Path contains unavailable parts\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (IOException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Directory path: " + path);
                e.Result = new BWorkerResultMessage("Directory Error", "Path is a file or unknown network\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (UnauthorizedAccessException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Directory path: " + path);
                e.Result = new BWorkerResultMessage("Directory Error", "No permission for this path\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Directory path: " + path);
                e.Result = new BWorkerResultMessage("Directory Error", "Path has an invalid format\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (NotSupportedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Directory path: " + path);
                e.Result = new BWorkerResultMessage("Directory Error", "Path contains unsupported characters\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Copy a local file
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="source_path">Source</param>
        /// <param name="dest_path">Destination</param>
        /// <param name="can_overwrite">true = overwrites if file already exists</param>
        /// <returns>"true" if successful</returns>
        public static bool CopyFile(DoWorkEventArgs e, string source_path, string dest_path, bool can_overwrite)
        {
            try
            {
                File.Copy(source_path, dest_path, can_overwrite);
            }
            catch (PathTooLongException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Source path: " + source_path);
                Console.WriteLine("Destination path: " + dest_path);
                e.Result = new BWorkerResultMessage("Copy Error", "Paths too long\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (FileNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Source path: " + source_path);
                Console.WriteLine("Destination path: " + dest_path);
                e.Result = new BWorkerResultMessage("Copy Error", "Source file not found\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (DirectoryNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Source path: " + source_path);
                Console.WriteLine("Destination path: " + dest_path);
                e.Result = new BWorkerResultMessage("Copy Error", "Paths contain unavailable parts\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (IOException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Source path: " + source_path);
                Console.WriteLine("Destination path: " + dest_path);
                e.Result = new BWorkerResultMessage("Copy Error", "Failed due to IO error\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (UnauthorizedAccessException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Source path: " + source_path);
                Console.WriteLine("Destination path: " + dest_path);
                e.Result = new BWorkerResultMessage("Copy Error", "No permission for copying\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Source path: " + source_path);
                Console.WriteLine("Destination path: " + dest_path);
                e.Result = new BWorkerResultMessage("Copy Error", "Paths have an invalid format\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (NotSupportedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Source path: " + source_path);
                Console.WriteLine("Destination path: " + dest_path);
                e.Result = new BWorkerResultMessage("Copy Error", "Paths contain unsupported characters\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Write text lines to a file (creates and eventually overwrites the file)
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="path">File path</param>
        /// <param name="contents">Text lines</param>
        /// <returns>"true" if successful</returns>
        public static bool WriteLinesToFile(DoWorkEventArgs e, string path, IEnumerable<string> contents)
        {
            try
            {
                File.WriteAllLines(path, contents);
            }
            catch (PathTooLongException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Write Error", "Path too long\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (DirectoryNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Write Error", "Path contain unavailable parts\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (IOException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Write Error", "IO error while opening file\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (UnauthorizedAccessException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Write Error", "No permission for file access\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Write Error", "Path has an invalid format\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (NotSupportedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Write Error", "Path contains unsupported characters\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SecurityException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Write Error", "Permission required\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Append text lines to a file (creates the file if it does not exist)
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="path">File path</param>
        /// <param name="contents">Text lines</param>
        /// <returns>"true" if successful</returns>
        public static bool AppendLinesToFile(DoWorkEventArgs e, string path, IEnumerable<string> contents)
        {
            try
            {
                File.AppendAllLines(path, contents);
            }
            catch (PathTooLongException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "Path too long\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (FileNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "Path not found\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (DirectoryNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "Path contain unavailable parts\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (IOException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "IO error while opening file\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (UnauthorizedAccessException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "No permission for file access\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "Path has an invalid format\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (NotSupportedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "Path contains unsupported characters\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SecurityException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Path: " + path);
                e.Result = new BWorkerResultMessage("Append Error", "Permission required\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }
    }
}
