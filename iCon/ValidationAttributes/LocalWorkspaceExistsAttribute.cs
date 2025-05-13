using System;
using System.ComponentModel.DataAnnotations;
using System.IO;

namespace iCon_General
{
    /// <summary>
    /// Custom validation attribute for checking if a local workspace directory exists
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false)]
    sealed public class LocalWorkspaceExistsAttribute : ValidationAttribute
    {
        /// <summary>
        /// Checks local directory existence
        /// </summary>
        public override bool IsValid(object value)
        {
            if (value == null) return false;
            if ((value is string) == false) return false;

            string workspace = (string)value;

            return Directory.Exists(TVMGUISettings.GetLocalWorkspacePath(workspace));
        }
    }
}
