using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.ComponentModel.DataAnnotations;
using System.Reflection;

namespace iCon_General
{
    /// <summary>
    /// Base class for all viewmodel objects that implements INotifyPropertyChanged (for view <-> viewmodel binding) 
    /// and INotifyDataErrorInfo (for property and full object validation with data attributes)
    /// </summary>
    public class ViewModelObject: INotifyPropertyChanged, INotifyDataErrorInfo
    {
        #region Implementation of INotifyPropertyChanged

        /// <summary>
        /// Event that occurs when a property value changes
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary> 
        /// Signals property change to binding objects by firing the PropertyChanged event 
        /// </summary>
        /// <param name="PropertyName"> Name of the changed property </param>
        protected void Notify(string PropertyName)
        {
            if (this.PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(PropertyName));
            }
        }

        #endregion Implementation of INotifyPropertyChanged

        // ******************************************************************* //
        // *  Many thanks to Mark Monster for sharing this smooth            * //
        // *  implementation of property validation in his blog.             * //
        // *  http://mark.mymonster.nl/2011/02/22/validating-our-viewmodel   * //
        // ******************************************************************* //

        #region Implementation of INotifyDataErrorInfo

        /// <summary>
        /// Event that occurs when the validation errors have changed for a property or for the entire entity
        /// </summary>
        public event EventHandler<DataErrorsChangedEventArgs> ErrorsChanged;

        /// <summary>
        /// Internal dictionary that holds the error messages for each property name
        /// </summary>
        private readonly IDictionary<string, IList<string>> _Errors = new Dictionary<string, IList<string>>();

        /// <summary>
        /// Gets the validation errors for a specified property or for the entire entity
        /// </summary>
        public IEnumerable GetErrors(string PropertyName)
        {
            if (_Errors.ContainsKey(PropertyName))
            {
                IList<string> propertyErrors = _Errors[PropertyName];
                foreach (string propertyError in propertyErrors)
                {
                    yield return propertyError;
                }
            }
            yield break;
        }

        /// <summary>
        /// Check if an error is registered for a property
        /// </summary>
        public bool HasPropertyErrors(string PropertyName)
        {
            return _Errors.ContainsKey(PropertyName);
        }

        /// <summary>
        /// Gets a value that indicates whether the entity has validation errors
        /// </summary>
        public bool HasErrors
        {
            get
            {
                return _Errors.Count > 0;
            }
        }

        /// <summary>
        /// Signals error changes to binding objects by firing the ErrorsChanged event
        /// </summary>
        /// <param name="PropertyName">Name of the property whose errors changed</param>
        protected void NotifyErrorsChanged(string PropertyName)
        {
            if (this.ErrorsChanged != null)
            {
                ErrorsChanged(this, new DataErrorsChangedEventArgs(PropertyName));
            }
        }

        #endregion Implementation of INotifyDataErrorInfo

        #region Validation Methods

        /// <summary>
        /// Add errors for a property to the dictionary
        /// </summary>
        private void AddErrorsForProperty(string PropertyName, IEnumerable<ValidationResult> ValidationResults)
        {
            RemoveErrorsForProperty(PropertyName);
            _Errors.Add(PropertyName, ValidationResults.Select(vr => vr.ErrorMessage).ToList());
        }

        /// <summary>
        /// Remove errors for a property from the dictionary
        /// </summary>
        private void RemoveErrorsForProperty(string propertyName)
        {
            if (_Errors.ContainsKey(propertyName))
            {
                _Errors.Remove(propertyName);
            }
        }

        /// <summary>
        /// Validates a value with the data annotations of a property
        /// </summary>
        /// <returns>true = the value is valid for this property</returns>
        protected bool ValidateProperty(string PropertyName, object Value)
        {
            ViewModelObject objectToValidate = this;
            var results = new List<ValidationResult>();
            bool isValid = Validator.TryValidateProperty(
                Value,
                new ValidationContext(objectToValidate, null, null)
                {
                    MemberName = PropertyName
                },
                results);

            if (isValid)
                RemoveErrorsForProperty(PropertyName);
            else
                AddErrorsForProperty(PropertyName, results);

            NotifyErrorsChanged(PropertyName);

            return isValid;
        }

        /// <summary>
        /// Validates all values of the properties with data annotations of this object
        /// </summary>
        /// <returns>true = all properties are valid</returns>
        public bool ValidateObject()
        {
            ViewModelObject objectToValidate = this;
            _Errors.Clear();
            Type objectType = objectToValidate.GetType();
            PropertyInfo[] properties = objectType.GetProperties();
            foreach (PropertyInfo property in properties)
            {
                if (property.GetCustomAttributes(typeof(ValidationAttribute), true).Any())
                {
                    object value = property.GetValue(objectToValidate, null);
                    ValidateProperty(property.Name, value);
                }
            }

            return !HasErrors;
        }

        #endregion Validation Methods

        #region Combined Validation-Notification Methods

        /// <summary>
        /// Validate a new property value, set the new value and notify of property changes
        /// </summary>
        /// <param name="PropertyName">Name of the property</param>
        /// <param name="Value">New value for the property</param>
        /// <param name="_Value">Background field</param>
        /// <returns>true = property has changed to a valid new value</returns>
        protected bool ValidateNotify(string PropertyName, double Value, ref double _Value)
        {
            bool result = false;
            if (Value.Equals(_Value) == false)
            {
                if (ValidateProperty(PropertyName, Value) == true) result = true;
                _Value = Value;
                Notify(PropertyName);
            }
            return result;
        }

        /// <summary>
        /// Validate a new property value, set the new value and notify of property changes
        /// </summary>
        /// <param name="PropertyName">Name of the property</param>
        /// <param name="Value">New value for the property</param>
        /// <param name="_Value">Background field</param>
        /// <returns>true = property has changed to a valid new value</returns>
        protected bool ValidateNotify(string PropertyName, int Value, ref int _Value)
        {
            bool result = false;
            if (Value.Equals(_Value) == false)
            {
                if (ValidateProperty(PropertyName, Value) == true) result = true;
                _Value = Value;
                Notify(PropertyName);
            }
            return result;
        }

        /// <summary>
        /// Validate a new property value, set the new value and notify of property changes
        /// </summary>
        /// <param name="PropertyName">Name of the property</param>
        /// <param name="Value">New value for the property</param>
        /// <param name="_Value">Background field</param>
        /// <returns>true = property has changed to a valid new value</returns>
        protected bool ValidateNotify(string PropertyName, long Value, ref long _Value)
        {
            bool result = false;
            if (Value.Equals(_Value) == false)
            {
                if (ValidateProperty(PropertyName, Value) == true) result = true;
                _Value = Value;
                Notify(PropertyName);
            }
            return result;
        }

        /// <summary>
        /// Validate a new property value, set the new value and notify of property changes
        /// </summary>
        /// <param name="PropertyName">Name of the property</param>
        /// <param name="Value">New value for the property</param>
        /// <param name="_Value">Background field</param>
        /// <returns>true = property has changed to a valid new value</returns>
        protected bool ValidateNotify(string PropertyName, string Value, ref string _Value)
        {
            bool result = false;
            if (Value != null)
            {
                string str_val = Value.Trim();
                if (str_val.Equals(_Value, StringComparison.Ordinal) == false)
                {
                    if (ValidateProperty(PropertyName, str_val) == true) result = true;
                    _Value = str_val;
                    Notify(PropertyName);
                }
            }
            return result;
        }

        #endregion Combined Validation-Notification Methods
    }
}
