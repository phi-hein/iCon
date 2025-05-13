using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace iCon_General
{
    public class FullSubmitPathConverterClass : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            bool is_remote_calc = (bool)values[0];
            string base_dir = (string)values[1];
            string job_prefix = (string)values[2];
            string remote_workspace = (string)values[3];
            string local_workspace = (string)values[4];

            return TVMSettings.GetFullSubmitPath(is_remote_calc, base_dir, job_prefix, remote_workspace, local_workspace);
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
