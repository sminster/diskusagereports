/* 
 * Copyright (c) 2011 Andr� Mekkawi <contact@andremekkawi.com>
 * 
 * LICENSE
 * 
 * This source file is subject to the MIT license in the file LICENSE.txt.
 * The license is also available at http://diskusagereport.sf.net/license.html
 */

;(function($){

$.extend(Controller.prototype, {
	
	// The default language, if the requested language is not supported.
	defaultLanguage: 'en-us',
	
	// The current language.
	language: null,
	
	// The language data.
	_languages: { },
	
	translate: function() {
		arguments = $.makeArray(arguments);
		
		if (!this._languages[this.language]) {
			throw "The language file for '" + this.language + "' has not been loaded.";
		}
		
		var key = arguments.shift(),
			str = this._languages[this.language][key],
			parts = [], isTextOnly = true,
			argIndex, lastIndex = 0, match, re = new RegExp('{([0-9])}', 'g');
		
		// Throw an error because the key does not exist.
		if (!$.isString(str)) {
			throw "'" + key + "' does not exist in language file.";
		}
		else {
			// TODO: Remove toUpperCase() debug code.
			//str = str.toUpperCase();
			
			// Find all replacements in the string.
			while (match = re.exec(str)) {
				
				// Add any text between the last match and this one.
				if (lastIndex < match.index) {
					parts.push(document.createTextNode(str.substring(lastIndex, match.index)));
				}
				
				argIndex = parseInt(match[1]) - 1;
				
				if ($.isUndefined(arguments[argIndex]) || arguments[argIndex] == null) {
					throw 'Replacement not passed to Controller.translate() for ' + match[0] + ' in ' + key;
				}
				else if ($.isString(arguments[argIndex]) || $.isNumber(arguments[argIndex])) {
					parts.push(document.createTextNode(arguments[argIndex]+''));
				}
				else if (arguments[argIndex].toArray) {
					parts.push.apply(parts, arguments[argIndex].toArray());
					isTextOnly = false;
				}
				else {
					throw 'Invalid argument for Controller.translate(). Must be string or jQuery object.';
				}
				
				lastIndex = re.lastIndex;
			}
			
			if (lastIndex != str.length) {
				parts.push(document.createTextNode(str.substr(lastIndex)));
			}
			
			if (isTextOnly) {
				return $(parts).text();
			}
			else {			
				return $(parts);
			}
		}
	},
	
	isSupportedLanguage: function(lang) {
		return $.isDefined(this._languages[lang.toLowerCase()]);
	},
	
	isLanguageLoaded: function(lang) {
		return this.isSupportedLanguage(lang) && this._languages[lang] != 'load';
	},
	
	addLanguage: function(lang) {
		if ($.isString(lang)) lang = [ lang ];
		
		for (var i = 0; i < lang.length; i++)
			if ($.isUndefined(this._languages[lang[i].toLowerCase()]))
				this._languages[lang[i].toLowerCase()] = 'load';
	},
	
	setLanguage: function(rules, returnFn) {
		var self = this,
			matches = this.matchSupportedLanguagesToRules(rules);
		
		// Set the lang to the first supported langauge match.
		// Otherwise set it to the language default.
		var lang = matches.length ? matches[0].lang : this.defaultLanguage;
		
		// Retrieve the language data if it has not been loaded.
		if (this._preLoad) {
			this.language = lang;
		}
		else {
			this.loadLanguage(lang, function(ret, message){
				if (ret) {
					self.language = lang;
					self._languageChangeStatic();
				}
				
				returnFn(ret, message);
			});
		}
	},
	
	loadLanguage: function(lang, returnFn) {
		var self = this;
		
		// Return that the language is unsupported.
		if (!this.isSupportedLanguage(lang)) {
			if ($.isFunction(returnFn))
				returnFn(false, 'Unsupported language: ' + lang);
		}
		
		// Retrieve the language data if it has not been loaded.
		else if (this._languages[lang] == 'load') {
			try {
				if (this._langXHR) this._langXHR.abort();
				this._langXHR = $.ajax({
					url: 'lang/' + lang + '.json',
					dataType: 'json',
					error: function(xhr, status, ex) {
						if (status == 'abort') return;
						
						if ($.isFunction(returnFn))
							returnFn(false, 'Failed to load language file (' + status + '): lang/' + lang + '.json');
					},
					success: function(data, status, xhr) {
						if (data) {
							if ($.isString(data['extends'])) {
								self.loadLanguage(data['extends'], function(ret, message){
									self._languages[lang] = $.extend({}, self._languages[data['extends']], data);
									returnFn(ret, message);
								});
							}
							else {
								if ($.isFunction(returnFn))
									self._languages[lang] = data;
									returnFn(true);
							}
						}
					}
				});
			}
			catch (e) {
				// TODO: Handle exception when data is viewed via 'file:///' protocol.
				if ($.isFunction(returnFn))
					returnFn(false, 'Failed to load language file (AJAX exception): lang/' + lang + '.json');
			}
		}
		
		// The data has already been loaded.
		else {
			if ($.isFunction(returnFn))
				returnFn(true);
		}
	},
	
	_languageChangeStatic: function(part) {
		if (!part || part == 'title') {
			if (this.settings && this.settings.name) {
				$('#Title').html(this.translate('title_with_name', $('<b>').text(this.settings.name))).show();
				document.title = this.translate('doctitle_with_name', this.settings.name);
			}
			else {
				$('#Title').html(this.translate('title')).show();
				document.title = this.translate('doctitle');
			}
		}
		
		if (!part || part == 'errors') {
			$('#ErrorsTitle span').html(this.translate('errors_title'));
			$('#ErrorsTitle div').html(this.translate('errors_close'));
		}
		
		if (!part || part == 'tree') {
			$('#TreeSort .tree-sortby-label span').html(this.translate('tree_header_name'));
			$('#TreeSort .tree-sortby-byte span').html(this.translate('tree_header_size'));
			$('#TreeSort .tree-sortby-num span').html(this.translate('tree_header_count'));
			$('#DirectoryTree li.ui-tree-files span.ui-tree-label').html(this.translate('tree_files_in_this_directory'));
		}
		
		if (!part || part == 'tabs') {
			$('#Tab_SubDirs').html(this.translate('tab_contents'));
			$('#Tab_Files').html(this.translate('tab_file_list'));
			$('#Tab_Modified').html(this.translate('tab_last_modified'));
			$('#Tab_Sizes').html(this.translate('tab_file_sizes'));
			$('#Tab_Types').html(this.translate('tab_file_types'));
			$('#Tab_Top100').html(this.translate('tab_top_100'));
		}
		
		if (!part || part == 'pager') {
			$('#Sections .pager-prev').html(this.translate('prev_page_button'));
			$('#Sections .pager-next').html(this.translate('next_page_button'));
		}
		
		if (!part || part == 'sections') {
			$('#SubDirs > thead .totals-sortby-label span').html(this.translate('contents_header_name'));
			$('#SubDirs > thead .totals-sortby-byte span').html(this.translate('contents_header_total_size'));
			$('#SubDirs > thead .totals-sortby-num span').html(this.translate('contents_header_file_count'));
			$('#SubDirs > tfoot td:eq(0)').html(this.translate('table_footer_total'));
			
			$('#Files-SortBy-name span').html(this.translate('file_list_header_name'));
			$('#Files-SortBy-type span').html(this.translate('file_list_header_type'));
			$('#Files-SortBy-size span').html(this.translate('file_list_header_size'));
			$('#Files-SortBy-modified span').html(this.translate('file_list_header_modified'));
			
			$('#Modified > thead .totals-sortby-label span').html(this.translate('last_modified_header_age'));
			$('#Modified > thead .totals-sortby-byte span').html(this.translate('last_modified_header_total_size'));
			$('#Modified > thead .totals-sortby-num span').html(this.translate('last_modified_header_file_count'));
			$('#Modified > tfoot td:eq(0)').html(this.translate('table_footer_total'));
			
			$('#Sizes > thead .totals-sortby-label span').html(this.translate('file_sizes_header_range'));
			$('#Sizes > thead .totals-sortby-byte span').html(this.translate('file_sizes_header_total_size'));
			$('#Sizes > thead .totals-sortby-num span').html(this.translate('file_sizes_header_file_count'));
			$('#Sizes > tfoot td:eq(0)').html(this.translate('table_footer_total'));
			
			$('#Types > thead .totals-sortby-label span').html(this.translate('file_types_header_extension'));
			$('#Types > thead .totals-sortby-byte span').html(this.translate('file_types_header_total_size'));
			$('#Types > thead .totals-sortby-num span').html(this.translate('file_types_header_file_count'));
			$('#Types > tfoot td:eq(0)').html(this.translate('table_footer_total'));
			
			$('#Top100-SortBy-name span').html(this.translate('top_100_header_name'));
			$('#Top100-SortBy-type span').html(this.translate('top_100_header_type'));
			$('#Top100-SortBy-size span').html(this.translate('top_100_header_size'));
			$('#Top100-SortBy-modified span').html(this.translate('top_100_header_modified'));
			$('#Top100-SortBy-path span').html(this.translate('top_100_header_path'));
		}
		
		if (!part || part == 'footer') {
			$('#Footer').html(this.translate(this.settings ? 'footer_with_created' : 'footer',
				this.settings ? this.settings.created.htmlencode() : null,
				$('<a target="_blank" href="http://diskusagereport.sourceforge.net/">Disk Usage Reports</a>'),
				$('<span id="ChangeLanguage">').text(this._languages[this.language].language_name)
			));
		}
		
		// Resize window in case the header was previously hidden.
		this.resizeWindow();
	},
	
	getSupportedLanguages: function() {
		return $.getArrayKeys(this._languages);
	},
	
	parseAcceptLanguage: function(rules) {
		// Remove whitespace and split into rules.
		rules = rules.toLowerCase().replace(/\s/g, '').split(',');
		
		for (var i = rules.length - 1; i >= 0; i--) {
			var split = rules[i].match(/^(([a-z]{1,8})(-([a-z]{1,8}))?)(;q=((1(\.[0]{1,3})?)|(0(\.[0-9]{1,3})?)))?$/i);
			// 1 = full language tag
			// 2 = primary language tag
			// 4 = secondary language tag
			// 6 = quality
			
			if (split) {
				rules[i] = { order: i, full: split[1], primary: split[2], secondary: split[4] ? split[4] : null, quality: isNaN(split[6]) ? 1 : parseFloat(split[6]) };
			}
			else {
				rules.splice(i, 1);
			}
		}
		
		this.sortLanguageRules(rules);
		
		return rules;
	},
	
	sortLanguageRules: function(rules) {
		rules.sort(function(a,b){
			var acomp = a.lang ? a.lang == a.full ? 0 : 1 : 0;
			var bcomp = b.lang ? b.lang == b.full ? 0 : 1 : 0;
			if (a.order == b.order && acomp != bcomp) {
				return acomp - bcomp;
			}
			else if (a.quality == 0 || b.quality == 0 || a.quality == b.quality) {
				return a.order  - b.order;
			}
			else {
				return b.quality - a.quality;
			}
		});
	},
	
	parseLanguageTag: function(lang) {
		var match = lang.replace(/\s/g, '').match(/^(([a-z]{1,8})(-([a-z]{1,8}))?)$/i);
		if (match)
			return { full: match[1], primary: match[2], secondary: match[4] ? match[4] : null };
		else
			return false;
	},
	
	determineRuleForLanguage: function(rules, lang) {
		var langMatch = this.parseLanguageTag(lang),
			ret = false;
		
		if (langMatch !== false) {
			for (var i = 0; i < rules.length; i++) {
				// The rule is an exact match.
				if (rules[i].full == langMatch.full) {
					return rules[i];
				}
				
				// The primary part of the language tag matches.
				else if (rules[i].primary == langMatch.primary) {
					// Our first partial match. 
					if (ret === false) {
						ret = rules[i];
					}
					// Only override the current partial match if it is more general.
					// Ex: For the lang 'en-xx', 'en' would override 'en-yy', but not vise vera.
					else if (ret.secondary != null && rules[i].secondary == null) {
						ret = rules[i];
					}
				}
			}
		}
		return ret;
	},
	
	matchSupportedLanguagesToRules: function(rules) {
		var supported = this.getSupportedLanguages();
		
		// Parse the rules as an Accept-Language header if it is a string.
		if ($.isString(rules)) rules = this.parseAcceptLanguage(rules);
		
		// Go through the list of supported languages and determine what matches the language rules.
		for (var i = supported.length - 1; i >= 0; i--) {
			var matchedRule = this.determineRuleForLanguage(rules, supported[i]);
			if (matchedRule === false || matchedRule.quality == 0) {
				supported.splice(i, 1);
			}
			else {
				supported[i] = $.extend({ lang: supported[i] }, matchedRule);
			}
			
		}
		
		this.sortLanguageRules(supported);
		
		return supported;
	}
});

})(jQuery);