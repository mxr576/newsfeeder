<?php
/**
 * @file
 * Contains the definition of the Find Keywords Feed Enhancer plugin.
 */

/**
 * Class FindKeywordsFeedItemEnhancerPlugin.
 *
 * Enhancer plugin, which finds the keywords in the content of the feed item
 * with the help of a dummy Node JS application.
 */
class FindKeywordsNodeFeedItemEnhancerPlugin extends NodeFeedItemEnhancerPlugin {
  /**
   * {@inheritdoc}
   */
  protected function enhancerProperties() {
    return [
      'keywords' => [
        'label' => t('Keywords'),
        'description' => '',
        'property_types' => ['list<taxonomy_term>'],
      ],
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function configurationForm() {
    $form['proxy_settings'] = [
      '#type' => 'fieldset',
      '#title' => t('Find Keyword Proxy settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    ];

    $form['proxy_settings']['proxy_address'] = [
      '#title' => t('Find Keyword Proxy server address'),
      '#description' => t('The address of the Find keyword Proxy server. Default is %default.', [
        '%default' => 'http://localhost:9999',
      ]),
      '#type' => 'textfield',
      '#default_value' => isset($this->settings['proxy_settings']['proxy_address']) ? $this->settings['proxy_settings']['proxy_address'] : 'http://localhost:9999',
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
    ];

    $form['proxy_settings']['timeout'] = [
      '#title' => t('Timeout'),
      '#description' => t('Maximum number of seconds while the Drupal wait for the response of the proxy.'),
      '#type' => 'select',
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
      '#options' => drupal_map_assoc([30, 60, 90, 120]),
      '#default_value' => isset($this->settings['proxy_settings']['timeout']) ? $this->settings['proxy_settings']['timeout'] : 30,
    ];

    $vocabularies = entity_load('taxonomy_vocabulary');

    $vocabularies = array_map(function ($vocab) {
      return $vocab->name;
    }, $vocabularies);

    $form['proxy_settings']['vocabularies'] = [
      '#title' => t('Keyword vocabulary'),
      '#description' => t("Select the vocabulary, which terms will be searched in the Feed item content."),
      '#type' => 'select',
      '#empty_option' => t('- None -'),
      '#options' => $vocabularies,
      '#default_value' => isset($this->settings['proxy_settings']['vocabularies']) ? $this->settings['proxy_settings']['vocabularies'] : '',
      '#disabled' => count($vocabularies) == 0,
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
    ];

    $text_fields = $this->getFeedItemNodePropertiesWithType('text_formatted');
    $text_fields += $this->getFeedItemNodePropertiesWithType('text_long');
    $text_fields += $this->getFeedItemNodePropertiesWithType('text');

    $text_fields = array_map(function ($field) {
      return $field['label'];
    }, $text_fields);


    $form['proxy_settings']['text_field'] = [
      '#title' => t('Find keywords in'),
      '#description' => t("Select which text field should be searched for keywords."),
      '#type' => 'select',
      '#empty_option' => t('- None -'),
      '#options' => $text_fields,
      '#default_value' => isset($this->settings['proxy_settings']['text_field']) ? $this->settings['proxy_settings']['text_field'] : '',
      '#disabled' => count($text_fields) == 0,
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
    ];

    $form['proxy_settings']['distance'] = [
      '#title' => t('Distance'),
      '#description' => t('The minimum distance of two words which should be handled as a match. See more details <a href="@url">here</a>.', ['@url' => 'https://en.wikipedia.org/wiki/Levenshtein_distance']),
      '#type' => 'select',
      '#empty_option' => t('- None -'),
      '#options' => $this->getDistanceOptions(),
      '#default_value' => isset($this->settings['proxy_settings']['distance']) ? $this->settings['proxy_settings']['distance'] : 3,
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
    ];

    return array_merge($form, parent::configurationForm());
  }

  /**
   * Return the Distance select field's possible option values.
   *
   * @return array
   *   Array of options.
   */
  private function getDistanceOptions() {
    $distance = [
      'Huge distance, less relevant results' => drupal_map_assoc([
        '10',
        '9',
        '8',
      ]),
    ];

    $distance += drupal_map_assoc([
      '7',
      '6',
      '5',
      '4',
    ]);

    $distance += [
      'Smaller distance, more relevent results' => drupal_map_assoc([
        '3',
        '2',
        '1',
        '0',
      ]),
    ];

    return $distance;
  }

  /**
   * {@inheritdoc}
   */
  public static function processFeedItem(EntityMetadataWrapper $feed_item_wrapper, array $settings) {
    // Keywords was not assigned to any field.
    if (empty($settings['properties']['keywords'])) {
      return TRUE;
    }

    if ($feed_item_wrapper->getPropertyInfo($settings['properties']['keywords'])) {
      $keywords = self::getTermsFromTheVocabulary($settings['proxy_settings']['vocabularies']);
      $text = $feed_item_wrapper->{$settings['proxy_settings']['text_field']}->value();
      if (is_array($text)) {
        $text = $feed_item_wrapper->{$settings['proxy_settings']['text_field']}->value->value();
      }
      $text = check_plain($text);
      // Full Content successfully extracted from the feed item's URL, save
      // it to the assigned field.
      $keywords_found = self::getKeywordsFromTheText($text, $keywords, $settings['proxy_settings']);
      if ($keywords_found) {
        $field_info = field_info_field($settings['properties']['keywords']);
        $keywords_found_tids = static::getTaxonomyTermReferenceFieldValues($keywords_found, $field_info);
        // Validate full content value.
        if ($feed_item_wrapper->{$settings['properties']['keywords']}->validate($keywords_found_tids)) {
          // Set the field value, but before that, remove all non-printable
          // characters from the string, which could cause DB error on
          // entity save.
          $feed_item_wrapper->{$settings['properties']['keywords']} = $keywords_found_tids;
        }
        else {
          watchdog(get_called_class(), 'Keywords can not be stored in the %field field, because they are invalid. Keywords: <pre>@keywords</pre>', [
            '%field' => $settings['properties']['keywords'],
            '@keywords' => print_r($keywords_found_tids, TRUE),
          ], WATCHDOG_WARNING);
        }
      }
      // TRUE, if full content retrieved and the Feed item's value
      // successfully updated with it, else FALSE.
      return $keywords && $feed_item_wrapper->save();
    }
    else {
      watchdog(get_called_class(), '%field field, which is assigned to Keywords does not exists on the feed item node anymore, that is why the value can not be saved.', [
        '%field' => $settings['properties']['keywords'],
      ], WATCHDOG_ERROR);
    }

    return FALSE;
  }

  /**
   * Helper function which returns all terms' name form the given vocab.
   *
   * @param int $vid
   *   Vocabulary ID.
   *
   * @return mixed
   *   Array of term names from the vocabulary.
   */
  private static function getTermsFromTheVocabulary($vid) {
    return db_select('taxonomy_term_data', 'ttd')
      ->fields('ttd', ['name'])
      ->condition('vid', $vid)
      ->addTag('translateble')
      ->execute()->fetchCol();
  }

  /**
   * Helper function which generate values for a term reference field.
   *
   * When a term does not exist it creates a new term in the proper
   * vocabulary.
   *
   * @param array $term_names
   *   Array of taxonomy term names.
   * @param array $field_info
   *   Field info, returned by field_get_info().
   *
   * @return array|bool
   *   Array of taxonomy term reference field values, FALSE if the field type
   *   does not match.
   */
  private static function getTaxonomyTermReferenceFieldValues(array $term_names, array $field_info) {
    // Ensure the field's type.
    if ($field_info['type'] !== 'taxonomy_term_reference') {
      return FALSE;
    }
    $vocabulary = $field_info['settings']['allowed_values'][0]['vocabulary'];
    // Check the field cardinality, limit the number of saved values
    // based on it.
    if ($field_info['cardinality'] != '-1') {
      $term_names = array_slice($term_names, $field_info['cardinality']);
    }
    return array_map(function ($term_name) use ($vocabulary) {
      $term = taxonomy_get_term_by_name($term_name, $vocabulary);

      // If the term does not exists on the vocabulary, create it.
      if (!$term) {
        $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary);
        $term = new stdClass();
        $term->vid = $vocabulary->vid;
        $term->name = $term_name;
        taxonomy_term_save($term);
      }

      $term = is_array($term) ? array_shift($term) : $term;
      return $term->tid;
    }, $term_names);
  }

  /**
   * Extract the matching keywords from the text.
   *
   * @param string $text
   *   Text to analyze.
   * @param array $keywords
   *   Keywords to search in the dex.
   * @param array $proxy_settings
   *   Settings of the proxy server which does the extraction.
   *
   * @return array|bool
   *   Returns the found keywords on success, else FALSE.
   */
  public static function getKeywordsFromTheText($text, array $keywords, array $proxy_settings) {
    $proxy_address = variable_get('find_keywords_in_text_proxy', $proxy_settings['proxy_address']);
    $response = drupal_http_request($proxy_address, [
      'method' => 'POST',
      'headers' => [
        'Accept' => 'application/json',
        'Content-type' => 'application/json',
      ],
      'data' => drupal_json_encode([
        'text' => $text,
        'keywords' => $keywords,
        'distance' => isset($proxy_settings['distance']) ? $proxy_settings['distance'] : 2,
      ]),
      'timeout' => !empty($proxy_settings['timeout']) ? $proxy_settings['timeout'] : 30,
    ]);

    if ($response->code == 200 && !empty($response->data)) {
      $data = drupal_json_decode($response->data);
      if (isset($data['keywords'])) {
        $keywords_found = array_map(function ($item) {
          return $item['keyword'];
        }, $data['keywords']);
        return array_unique($keywords_found);
      }
    }
    else {
      if (!empty($response->data)) {
        $response->data = drupal_json_decode($response->data);
      }
      watchdog(get_called_class(), 'Find Keywords Proxy returned with unknown status code or error. Proxy URL: %proxy_url, details: <pre>@details</pre>', [
        'proxy_url' => $proxy_address,
        '@details' => print_r($response, TRUE),
      ], WATCHDOG_WARNING);
    }

    return FALSE;
  }
}
