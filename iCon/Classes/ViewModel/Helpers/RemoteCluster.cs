using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Threading;
using Renci.SshNet;
using Renci.SshNet.Common;

namespace iCon_General
{
    /// <summary>
    /// Manages an SSH and SFTP connection to a remote cluster
    /// </summary>
    internal class RemoteCluster : IDisposable
    {
        #region Fields

        protected SshClient _sshcl;

        protected SftpClient _sftpcl;

        protected string _homedir;

        #endregion

        #region Properties

        /// <summary>
        /// Home directory path
        /// </summary>
        public string HomeDirectory
        {
            get
            {
                return _homedir;
            }
        }

        #endregion

        #region IDisposable implementation

        protected bool _disposed;

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    _sshcl?.Dispose();
                    _sshcl = null;
                    _sftpcl?.Dispose();
                    _sftpcl = null;
                }

                _disposed = true;
            }
        }

        public void Dispose()
        {
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }

        #endregion

        /// <summary>
        /// Initializes the internal SSH and SFTP clients
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="RemoteProfile">The selected remote profile</param>
        /// <returns>"true" if successful</returns>
        public bool Initialize(DoWorkEventArgs e, TVMGUISettingsRemoteProfile RemoteProfile)
        {
            _sshcl = CreateSSHClient(e, RemoteProfile);
            if (_sshcl == null) return false;

            _sftpcl = CreateSFTPClient(e, RemoteProfile);
            if (_sftpcl == null) return false;

            return true;
        }

        /// <summary>
        /// Establish the SSH and SFTP connections
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <returns>"true" if successful</returns>
        public bool Connect(DoWorkEventArgs e)
        {
            try
            {
                _sshcl.Connect();
                if (_sshcl.IsConnected == false)
                {
                    e.Result = new BWorkerResultMessage("Invalid Input", "Cannot connect to specified server\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return false;
                }

                _sftpcl.Connect();
                if (_sftpcl.IsConnected == false)
                {
                    e.Result = new BWorkerResultMessage("Invalid Input", "Cannot connect to specified server\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return false;
                }

                _homedir = _sftpcl.WorkingDirectory;
            }
            catch (SshConnectionException ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                e.Result = new BWorkerResultMessage("Connection Error", "SSH session could not be established\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SshAuthenticationException ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                e.Result = new BWorkerResultMessage("Authentication Error", "Authentication of SSH session failed\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SocketException ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                e.Result = new BWorkerResultMessage("SSH Socket Error", "Socket connection could not be established\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Checks whether remote file or directory exists
        /// </summary>
        public bool Exists(string path)
        {
            return _sftpcl.Exists(path);
        }

        /// <summary>
        /// Creates a remote directory (and all missing directories within the path)
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="path">Directory path (absolute or relative to home directory)</param>
        /// <returns>"true" if successful</returns>
        public bool CreateDirectory(DoWorkEventArgs e, string path)
        {
            // Remove home directory (if path starts with it)
            string dirpath = path;
            if (path.StartsWith(_homedir + "/"))
            {
                dirpath = path.Substring(_homedir.Length + 1);
            }

            // Create list of directories along the path
            List<string> dirs = RemotePaths.GetDirectoryList(dirpath);

            // Create directories
            foreach (string dir in dirs)
            {
                if (CreateSingleDirectory(e, dir) == false)
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Creates a remote directory (without intermediate directories within the path)
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="path">Directory path (absolute or relative to home directory)</param>
        /// <returns>"true" if successful</returns>
        public bool CreateSingleDirectory(DoWorkEventArgs e, string path)
        {
            string fullpath = RemotePaths.Combine(_homedir, path);

            try
            {
                if (_sftpcl.Exists(fullpath) == false)
                {
                    _sftpcl.CreateDirectory(fullpath);
                }
            }
            catch (SshConnectionException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while creating directory: " + fullpath + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "SSH connection is invalid\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SftpPermissionDeniedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while creating directory: " + fullpath + ")");
                e.Result = new BWorkerResultMessage("SFTP Error", "SFTP permission denied\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SshException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while creating directory: " + fullpath + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "An SSH error occured\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while creating directory: " + fullpath + ")");
                e.Result = new BWorkerResultMessage("SFTP Error", "Invalid path\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Executes a command via the internal SSH client (and prints the resulting output to the console)
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="cmd">Command</param>
        /// <returns>"true" if successful</returns>
        public bool ExecuteCommand(DoWorkEventArgs e, string cmd)
        {
            try
            {
                SshCommand ssh_cmd = _sshcl.CreateCommand(cmd);
                string result = ssh_cmd.Execute();

                if (string.IsNullOrWhiteSpace(result) == false)
                {
                    Console.WriteLine(result);
                }

                if (ssh_cmd.ExitStatus != 0)
                {
                    Console.WriteLine();
                    Console.WriteLine("Error: SSH command exited with a non-zero exit code.");
                    Console.WriteLine("Command: " + cmd);
                    Console.WriteLine("Exit code: " + ssh_cmd.ExitStatus.ToString());
                    if (string.IsNullOrWhiteSpace(ssh_cmd.Error) == false)
                    {
                        Console.WriteLine("Error output: " + ssh_cmd.Error);
                    }
                    e.Result = new BWorkerResultMessage("SSH Error", "SSH command exited with error\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return false;
                }
            }
            catch (SshConnectionException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while executing command: " + cmd + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "SSH connection is invalid\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SshOperationTimeoutException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while executing command: " + cmd + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "SSH connection timeout\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SshException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while executing command: " + cmd + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "Could not execute SSH command\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Executes a command via the internal SSH client (without printing to the console)
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="cmd">Command</param>
        /// <returns>command output if successful (null in case of error)</returns>
        public string ExecuteCommandSilent(DoWorkEventArgs e, string cmd)
        {
            try
            {
                SshCommand ssh_cmd = _sshcl.CreateCommand(cmd);
                string result = ssh_cmd.Execute();

                if (ssh_cmd.ExitStatus != 0)
                {
                    Console.WriteLine();
                    Console.WriteLine("Error: SSH command exited with a non-zero exit code.");
                    Console.WriteLine("Command: " + cmd);
                    Console.WriteLine("Exit code: " + ssh_cmd.ExitStatus.ToString());
                    if (string.IsNullOrWhiteSpace(ssh_cmd.Error) == false)
                    {
                        Console.WriteLine("Error output: " + ssh_cmd.Error);
                    }
                    e.Result = new BWorkerResultMessage("SSH Error", "SSH command exited with error\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }

                return result;
            }
            catch (SshConnectionException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while executing command: " + cmd + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "SSH connection is invalid\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return null;
            }
            catch (SshOperationTimeoutException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while executing command: " + cmd + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "SSH connection timeout\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return null;
            }
            catch (SshException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("(while executing command: " + cmd + ")");
                e.Result = new BWorkerResultMessage("SSH Error", "Could not execute SSH command\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return null;
            }
        }

        /// <summary>
        /// Uploads a local file to a remote destination
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="local_path">Local file path</param>
        /// <param name="remote_path">Remote file path</param>
        /// <param name="permissions">Permission specifier (e.g. 744)</param>
        /// <returns>"true" if successful</returns>
        public bool UploadFile(DoWorkEventArgs e, string local_path, string remote_path, short permissions)
        {
            try
            {
                using (FileStream local_stream = File.OpenRead(local_path))
                {
                    _sftpcl.UploadFile(local_stream, remote_path, true);
                    _sftpcl.ChangePermissions(remote_path, permissions);
                }
            }
            catch (SshConnectionException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SSH Error", "Transmission connection is invalid\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SftpPermissionDeniedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SFTP Error", "Transmission permission denied\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SftpPathNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SFTP Error", "Transmitted file not found\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SshException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SFTP Error", "File transmission failed\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SFTP Error", "Invalid file transmission path\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (PathTooLongException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("File Error", "Local transmission path too long\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (DirectoryNotFoundException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("File Error", "Local transmission directory not found\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (UnauthorizedAccessException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("File Error", "Unauthorized access to local file\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (FileNotFoundException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("File Error", "Local transmission file not found\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (NotSupportedException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("File Error", "Invalid local transmission file format\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (IOException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Local path: " + local_path);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("File Error", "Cannot open local transmission file\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Create a text file on the remote system
        /// </summary>
        /// <param name="e">Background worker event args</param>
        /// <param name="remote_path">Remote file path</param>
        /// <param name="contents">Text of the file</param>
        /// <param name="permissions">Permission specifier (e.g. 744)</param>
        /// <returns></returns>
        public bool CreateTextFile(DoWorkEventArgs e, string remote_path, string contents, short permissions)
        {
            try
            {
                _sftpcl.WriteAllText(remote_path, contents);
                _sftpcl.ChangePermissions(remote_path, permissions);
            }
            catch (SshConnectionException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SSH Error", "File creation connection is invalid\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SftpPermissionDeniedException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SFTP Error", "Permission denied for created file\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            catch (SftpPathNotFoundException ex)
            {
                Console.WriteLine("");
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SFTP Error", "Invalid path of created file\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }
            
            catch (SshException ex)
            {
                Console.WriteLine();
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Remote path: " + remote_path);
                e.Result = new BWorkerResultMessage("SFTP Error", "File creation failed\n(see console for details)\n.",
                    Constants.KMCERR_INVALID_INPUT, false);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Create SshClient object
        /// </summary>
        /// <returns>SshClient object (if successful) or null (if error)</returns>
        protected static SshClient CreateSSHClient(DoWorkEventArgs e, TVMGUISettingsRemoteProfile RemoteProfile)
        {
            // Obtain ConnectionInfo
            var coninfo = CreateConnectionInfo(e, RemoteProfile, "SSH");
            if (coninfo == null)
            {
                if (e.Result == null)
                {
                    Console.WriteLine("Error: Creation of connection info object for SSH failed.");
                    e.Result = new BWorkerResultMessage("Unknown error", "SSH setup failed\n(see console for details)\n",
                    Constants.KMCERR_OBJECT_NOT_READY, false);
                }
                return null;
            }

            // Create client
            SshClient client = new SshClient(coninfo);

            // Set keep alive interval
            client.KeepAliveInterval = TimeSpan.FromSeconds(10);

            // Specify host key validation
            client.HostKeyReceived += delegate (object sender, HostKeyEventArgs eventargs)
            {
                // Primitive fingerprint check
                if (eventargs.FingerPrint.Length == 0)
                {
                    Console.WriteLine("Error: No host fingerprint received.");
                    eventargs.CanTrust = false;
                    return;
                }

                // Check against stored fingerprint
                if ((string.IsNullOrWhiteSpace(RemoteProfile.HostFingerPrint) == false) &&
                    (eventargs.FingerPrintSHA256 == RemoteProfile.HostFingerPrint))
                {
                    eventargs.CanTrust = true;
                    return;
                }

                // Show fingerprint dialog
                eventargs.CanTrust = (bool)Application.Current.Dispatcher.Invoke(
                    new Func<string, string, string, string, bool>((string hostname, string keytype, string keylength, string fingerprint) =>
                    {
                        return FingerPrintDialog.Prompt(hostname, keytype, keylength, fingerprint);
                    }),
                    DispatcherPriority.Normal,
                    RemoteProfile.HostAdress,
                    eventargs.HostKeyName,
                    eventargs.KeyLength.ToString(),
                    eventargs.FingerPrintSHA256);

                if (eventargs.CanTrust == true)
                {
                    RemoteProfile.HostFingerPrint = eventargs.FingerPrintSHA256;
                }
            };

            return client;
        }

        /// <summary>
        /// Create SftpClient object
        /// </summary>
        /// <returns>SftpClient object (if successful) or null (if error)</returns>
        protected static SftpClient CreateSFTPClient(DoWorkEventArgs e, TVMGUISettingsRemoteProfile RemoteProfile)
        {
            // Obtain ConnectionInfo
            var coninfo = CreateConnectionInfo(e, RemoteProfile, "SFTP");
            if (coninfo == null)
            {
                if (e.Result == null)
                {
                    Console.WriteLine("Error: Creation of connection info object for SFTP failed.");
                    e.Result = new BWorkerResultMessage("Unknown error", "SSH setup failed\n(see console for details)\n",
                    Constants.KMCERR_OBJECT_NOT_READY, false);
                }
                return null;
            }

            // Create client
            SftpClient client = new SftpClient(coninfo);

            // Set keep alive interval
            client.KeepAliveInterval = TimeSpan.FromSeconds(10);

            // Specify host key validation
            client.HostKeyReceived += delegate (object sender, HostKeyEventArgs eventargs)
            {
                // Primitive fingerprint check
                if (eventargs.FingerPrint.Length == 0)
                {
                    Console.WriteLine("Error: No host fingerprint received.");
                    eventargs.CanTrust = false;
                    return;
                }

                // Check against stored fingerprint
                if ((string.IsNullOrWhiteSpace(RemoteProfile.HostFingerPrint) == false) &&
                    (eventargs.FingerPrintSHA256 == RemoteProfile.HostFingerPrint))
                {
                    eventargs.CanTrust = true;
                    return;
                }

                // Show fingerprint dialog
                eventargs.CanTrust = (bool)Application.Current.Dispatcher.Invoke(
                    new Func<string, string, string, string, bool>((string hostname, string keytype, string keylength, string fingerprint) =>
                    {
                        return FingerPrintDialog.Prompt(hostname, keytype, keylength, fingerprint);
                    }),
                    DispatcherPriority.Normal,
                    RemoteProfile.HostAdress,
                    eventargs.HostKeyName,
                    eventargs.KeyLength.ToString(),
                    eventargs.FingerPrintSHA256);

                if (eventargs.CanTrust == true)
                {
                    RemoteProfile.HostFingerPrint = eventargs.FingerPrintSHA256;
                }
            };

            return client;
        }

        /// <summary>
        /// Creates ConnectionInfo object with server authentication settings
        /// </summary>
        /// <returns>ConnectionInfo object (if successful) or null (if error)</returns>
        protected static ConnectionInfo CreateConnectionInfo(DoWorkEventArgs e, TVMGUISettingsRemoteProfile RemoteProfile, string clienttype)
        {
            // Create desired authentication method list
            List<AuthenticationMethod> auth_methods = new List<AuthenticationMethod>();

            // Create password authentication method
            if (RemoteProfile.WithPassword == true)
            {
                // Add password authentication method
                try
                {
                    auth_methods.Add(new PasswordAuthenticationMethod(RemoteProfile.Username.Trim(),
                        RemoteProfile.UserPassword.Trim()));
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Error: User name (" + RemoteProfile.Username.Trim() + ") has zero length or contains only white space.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid user name\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
            }

            // Create private key authentication method
            if (RemoteProfile.WithPrivateKey == true)
            {
                // Check if private key file exists
                if (File.Exists(RemoteProfile.PrivateKeyPath.Trim()) == false)
                {
                    Console.WriteLine("Error: Private key file (" + RemoteProfile.PrivateKeyPath.Trim() + ") is missing.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Private key file does not exist\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }

                // Create keyfiles
                PrivateKeyFile keyfile = null;
                try
                {
                    keyfile = new PrivateKeyFile(RemoteProfile.PrivateKeyPath.Trim(),
                        RemoteProfile.PrivateKeyPassword.Trim());
                }
                catch (SshException ex)
                {
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file\n(see console for details)\n.",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
                catch (NotSupportedException ex)
                {
                    Console.WriteLine("Error: " + ex.Message);
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Error: Private key file path has zero length, contains only white space or contains one or more invalid characters.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
                catch (PathTooLongException)
                {
                    Console.WriteLine("Error: Private key file path exceeds the system-defined maximum path length.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
                catch (DirectoryNotFoundException)
                {
                    Console.WriteLine("Error: Directory of the private key file does not exist.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
                catch (UnauthorizedAccessException)
                {
                    Console.WriteLine("Error: Private key file access permissions prevent loading of the file.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
                catch (FileNotFoundException)
                {
                    Console.WriteLine("Error: Private key file not found.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
                catch (IOException)
                {
                    Console.WriteLine("Error: IO-error when opening private key file.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid private key file path\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }

                // Add key file authentication method
                try
                {
                    auth_methods.Add(new PrivateKeyAuthenticationMethod(RemoteProfile.Username.Trim(), keyfile));
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Error: User name (" + RemoteProfile.Username.Trim() + ") has zero length or contains only white space.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid user name\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
            }

            // Create keyboard-interactive authentication method
            if (RemoteProfile.WithKeyboardInteractive == true)
            {
                // Add keyboard-interactive auth method
                try
                {
                    var keyint_auth = new KeyboardInteractiveAuthenticationMethod(RemoteProfile.Username.Trim());
                    keyint_auth.AuthenticationPrompt += delegate (object sender, AuthenticationPromptEventArgs eventargs)
                    {
                        foreach (AuthenticationPrompt prompt in eventargs.Prompts)
                        {
                            if (string.IsNullOrWhiteSpace(prompt.Request)) continue;

                            if ((RemoteProfile.WithPassword == true) &&
                                (string.IsNullOrWhiteSpace(RemoteProfile.UserPassword) == false) &&
                                (prompt.Request.IndexOf("password", StringComparison.InvariantCultureIgnoreCase) != -1))
                            {
                                prompt.Response = RemoteProfile.UserPassword.Trim();
                            }
                            else
                            {
                                prompt.Response = (string)Application.Current.Dispatcher.Invoke(
                                    new Func<string, string, string>((string cltype, string question) =>
                                    {
                                        return AuthDialog.Prompt("Server request (" + cltype + ")", question);
                                    }), DispatcherPriority.Normal, clienttype, prompt.Request);
                            }
                        }
                    };
                    auth_methods.Add(keyint_auth);
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Error: User name (" + RemoteProfile.Username.Trim() + ") has zero length or contains only white space.");
                    e.Result = new BWorkerResultMessage("Invalid Input", "Invalid user name\n(see console for details)\n",
                        Constants.KMCERR_INVALID_INPUT, false);
                    return null;
                }
            }

            // Create connection info object
            ConnectionInfo coninfo = null;
            try
            {
                coninfo = new ConnectionInfo(RemoteProfile.HostAdress.Trim(), RemoteProfile.HostPort,
                    RemoteProfile.Username.Trim(), auth_methods.ToArray());
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine("Error: Invalid connection setting: " + ex.Message);
                e.Result = new BWorkerResultMessage("Invalid Input", "Invalid connection settings\n(see console for details)\n",
                    Constants.KMCERR_INVALID_INPUT, false);
                return null;
            }

            // Set connection timeout
            coninfo.Timeout = TimeSpan.FromMinutes(10);

            return coninfo;
        }
    }
}
