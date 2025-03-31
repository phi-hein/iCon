using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Xml;
using System.IO;

namespace iCon_General.Properties
{


    class AppSettings
    {
        public AppSettings()
        {
            settingsFile = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + "\\SampleApp\\Settings.xml";
        }

        public void Load()
        {
            try
            {
                settings = new XmlDocument();
                settings.Load(settingsFile);
            }
            catch (Exception)
            {

            }

            if (settings.SelectSingleNode("/Settings") == null)
                settings.AppendChild(settings.CreateElement("Settings", null));
        }

        public void Save()
        {
            try
            {
                string settingsPath = Path.GetDirectoryName(settingsFile);
                if (!Directory.Exists(settingsPath))
                {
                    Directory.CreateDirectory(settingsPath);
                }

                settings.Save(settingsFile);
            }
            catch (Exception)
            {

            }
        }

        public string GetProperty(string name)
        {
            XmlNode val = settings.SelectSingleNode("/Settings/" + name);

            if (val == null)
                return null;

            return val.InnerText;
        }

        public void SetProperty(string name, string value)
        {
            XmlNode val = settings.SelectSingleNode("/Settings/" + name);

            if (val != null)
                val.InnerText = value;
            else
            {
                val = settings.CreateNode(XmlNodeType.Element, name, null);
                val.InnerText = value;

                settings.DocumentElement.SelectSingleNode("/Settings").AppendChild(val);
            }
        }

        XmlDocument settings;
        //Dictionary<string, string> settings;
        string settingsFile;
    }

}