<?php
/**
 * @file
 * Contains the definition of the Full Content Feed Enhancer plugin.
 */

/**
 * Class FullContentFeedItemEnhancerPlugin.
 *
 * Retrieve the full content of the feed item from the feed item's source
 * link.
 */
class FullContentFeedItemEnhancerPlugin extends NodeFeedItemEnhancerPlugin {
  /**
   * {@inheritdoc}
   */
  protected function enhancerProperties() {
    return [
      'full_content' => [
        'label' => t('Full Content'),
        'description' => '',
        'property_types' => ['text_formatted', 'text_long'],
      ],
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function configurationForm() {
    $form = [];

    $form['proxy_settings'] = [
      '#type' => 'fieldset',
      '#title' => t('Full Content Proxy settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    ];

    $form['proxy_settings']['proxy_address'] = [
      '#title' => t('Full Content Proxy server address'),
      '#description' => t('The address of the Full Content Proxy server. Default is %default , if you using the <a href="@wce-api-url">WCE API.</a>', [
        '%default' => 'http://localhost:8001/?url=',
        '@wce-api-url' => 'https://www.npmjs.com/package/wce-api',
      ]),
      '#type' => 'textfield',
      '#default_value' => isset($this->settings['proxy_settings']['proxy_address']) ? $this->settings['proxy_settings']['proxy_address'] : 'http://localhost:8001/?url=',
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

    $form['proxy_settings']['number_of_tries'] = [
      '#title' => t('Maximum number of tries'),
      '#description' => t('Maximum number of tries while the Drupal try to import the full content of a feed item.'),
      '#type' => 'select',
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
      '#options' => drupal_map_assoc([5, 10, 25, 50, 75, 100]),
      '#default_value' => isset($this->settings['proxy_settings']['number_of_tries']) ? $this->settings['proxy_settings']['number_of_tries'] : 10,
    ];

    $link_fields = $this->getFeedItemNodePropertiesWithType('field_item_link');

    $link_fields = array_map(function ($field) {
      return $field['label'];
    }, $link_fields);

    $form['proxy_settings']['feed_item_url_field'] = [
      '#title' => t('Feed item url'),
      '#description' => t("Select the field, which stores each feed items' original urls. This field must be a link field."),
      '#type' => 'select',
      '#empty_option' => t('- None -'),
      '#options' => $link_fields,
      '#default_value' => isset($this->settings['proxy_settings']['feed_item_url_field']) ? $this->settings['proxy_settings']['feed_item_url_field'] : '',
      '#disabled' => count($link_fields) == 0,
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
    ];

    return array_merge($form, parent::configurationForm());
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormValidate(array $form, array &$form_state, array &$values) {
    parent::configurationFormValidate($form, $form_state, $values);

    if (!self::getFullContentOfUrl('http://cnn.com', $values['proxy_settings'])) {
      form_error($form['proxy_settings']['proxy_address'], t('Full Content Proxy is unreachable on the provided address or returned with wrong reply!'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public static function processFeedItem(EntityMetadataWrapper $feed_item_wrapper, array $settings) {
    // Full Content was not assigned to any field.
    if (empty($settings['properties']['full_content'])) {
      return TRUE;
    }

    if ($feed_item_wrapper->getPropertyInfo($settings['proxy_settings']['feed_item_url_field'])) {
      if ($feed_item_wrapper->getPropertyInfo($settings['properties']['full_content'])) {
        // If Full Content successfully extracted from the feed item's URL,
        // then save it to the assigned field.
        if ($full_content = self::getFullContentOfUrl($feed_item_wrapper->{$settings['proxy_settings']['feed_item_url_field']}->url->value(), $settings['proxy_settings'])) {
          $full_content = newsfeeder_hacks_clean_non_printable_utf_characters_from_string($full_content);
          // Validate full content value.
          if ($feed_item_wrapper->{$settings['properties']['full_content']}->validate($full_content)) {
            // Set the field value.
            $feed_item_wrapper->{$settings['properties']['full_content']}->value = $full_content;
          }
          else {
            watchdog(get_called_class(), 'Returned full content from <a href="@url">this url</a> can not be stored in the %field field, because it is invalid. Full content: <pre>@full_content</pre>', [
              '%field' => $settings['properties']['full_content'],
              '@url' => $feed_item_wrapper->{$settings['proxy_settings']['feed_item_url_field']}->url->value(),
              '@full_content' => print_r($full_content, TRUE),
            ], WATCHDOG_WARNING);
          }
        }
        else {
          watchdog(get_called_class(), 'Empty content returned by the Full Content Proxy from <a href="@url">this url</a>.', [
            '@url' => $feed_item_wrapper->{$settings['proxy_settings']['feed_item_url_field']}->url->value(),
          ], WATCHDOG_WARNING);
        }
        // TRUE, if full content retrieved and the Feed item's value
        // successfully updated with it, else FALSE.
        return $full_content && $feed_item_wrapper->save();
      }
      else {
        watchdog(get_called_class(), '%field field, which is assigned to Full Content value does not exists on the feed item node anymore, that is why the value can not be saved.', [
          '%field' => $settings['properties']['full_content'],
        ], WATCHDOG_ERROR);
      }
    }
    else {
      watchdog(get_called_class(), 'Feed item url field (%field) does not exists on the feed item node anymore.', [
        '%field' => $settings['proxy_settings']['feed_item_url_field'],
      ], WATCHDOG_ERROR);
    }

    return FALSE;
  }

  /**
   * Get the full content of the webpage.
   *
   * @param string $url
   *   URL of the webpage.
   * @param array $proxy_settings
   *   Settings of the Full Content Proxy.
   *
   * @return bool|string
   *   Content of the webpage on success, else FALSE.
   */
  public static function getFullContentOfUrl($url, array $proxy_settings) {
    // Allow global proxy address override.
    $proxy_address = empty($proxy_settings['proxy_address']) ? 'http://localhost:8001/?url=' : $proxy_settings['proxy_address'];
    $proxy_address = variable_get('full_content_enhancer_proxy_address', $proxy_address);
    $response = drupal_http_request($proxy_address . $url, [
      'method' => 'GET',
      'headers' => [
        'Accept' => 'application/json',
        'Content-type' => 'text/plain',
      ],
      'timeout' => !empty($proxy_settings['timeout']) ? $proxy_settings['timeout'] : 30,
    ]);

    if ($response->code == 200 && !empty($response->data)) {
      $data = drupal_json_decode($response->data);
      if (isset($data['content'])) {
        return $data['content'];
      }
      else {
        watchdog(get_called_class(), 'No content returned by the Full Content Proxy! URL: %url', [
          '%url' => $proxy_address . $url,
        ], WATCHDOG_WARNING);
        // Full Content Proxy wasn't and won't be able to extract the content
        // of the feed item (ex.: linked pdf files).
        return ' ';
      }
    }
    else {
      // Decode the returned error information.
      if (!empty($response->data)) {
        $response->data = drupal_json_decode($response->data);
      }
      watchdog(get_called_class(), 'Full Content Proxy returned with unknown status code or error. URL: %url, details: <pre>@details</pre>', [
        '%url' => $proxy_address . $url,
        '@details' => print_r($response, TRUE),
      ], WATCHDOG_WARNING);
    }

    return FALSE;
  }

}
