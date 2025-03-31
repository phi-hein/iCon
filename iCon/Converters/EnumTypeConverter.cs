using System;
using System.ComponentModel;
using System.Globalization;
using System.Linq;

namespace iCon_General
{
    // Copied from: http://blog.spencen.com/2009/04/19/binding-an-enum-property-to-a-combobox-using-customized-text.aspx

    /// <summary>
    /// Attribute for user friedly names of enum values
    /// </summary>
    [AttributeUsage(AttributeTargets.Field, AllowMultiple = false)]
    public class EnumDisplayNameAttribute : Attribute
    {
        public EnumDisplayNameAttribute(string displayName)
        {
            DisplayName = displayName;
        }

        public string DisplayName { get; set; }
    }

    /// <summary>
    /// Type converter for substituting EnumValue with EnumDisplayName (for binding enum to combobox)
    /// </summary>
    public class EnumTypeConverter : EnumConverter
    {
        public EnumTypeConverter(Type enumType) : base(enumType) { }

        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
        {
            if (destinationType == typeof(string) && value != null)
            {
                var enumType = value.GetType();
                if (enumType.IsEnum)
                    return GetDisplayName(value);
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        private string GetDisplayName(object enumValue)
        {
            var displayNameAttribute = EnumType.GetField(enumValue.ToString())
                                                                 .GetCustomAttributes(typeof(EnumDisplayNameAttribute), false)
                                                                 .FirstOrDefault() as EnumDisplayNameAttribute;
            if (displayNameAttribute != null)
                return displayNameAttribute.DisplayName;

            return Enum.GetName(EnumType, enumValue);
        }
    }
}
