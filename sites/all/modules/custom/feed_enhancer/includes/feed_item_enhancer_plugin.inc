<?php

/**
 * @file
 * Interface and abstract class definitions of the FeedItemEnhancerPlugin.
 */

/**
 * Interface FeedItemEnhancerPluginPluginInterface.
 *
 * Base interface of all Feed Item Enhancer plugins.
 */
interface FeedItemEnhancerPluginPluginInterface {

  /**
   * Construct a FeedItemEnhancerPlugin.
   *
   * @param \FeedsConfigurable $feeds_configurable
   *   Either FeedsImporter or FeedsSource object.
   * @param string $machine_name
   *   Machine name of the plugin.
   * @param array $settings
   *   Settings of the FeedItemEnhancerPlugin.
   */
  public function __construct(FeedsConfigurable $feeds_configurable, $machine_name, array $settings = []);

  /**
   * Display a configuration form for the plugin.
   *
   * @return array
   *   A form array for configuring this plugin, or FALSE if no configuration
   *   is possible.
   */
  public function configurationForm();

  /**
   * Validation callback for the form returned by configurationForm().
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $form_state
   *   The complete form state.
   * @param array $values
   *   The part of the $form_state['values'] array corresponding to this form.
   */
  public function configurationFormValidate(array $form, array &$form_state, array &$values);

  /**
   * Submit callback for the form returned by configurationForm().
   *
   * This method should both return the new options and set them internally.
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $form_state
   *   The complete form state.
   * @param array $values
   *   The part of the $form_state['values'] array corresponding to this form.
   *
   * @return array
   *   The new settings array for this plugins.
   */
  public function configurationFormSubmit(array $form, array &$form_state, array &$values);

  /**
   * Returns the settings of this plugin.
   *
   * @return array
   *   The settings of this plugin.
   */
  public function getSettings();

  /**
   * Process the feed item with this plugin.
   *
   * @param \EntityMetadataWrapper $feed_item_wrapper
   *   Entity meta data wrapper of the feed item.
   * @param array $settings
   *   Settings of the plugin.
   *
   * @return bool
   *   TRUE on succes, else FALSE.
   */
  public static function processFeedItem(EntityMetadataWrapper $feed_item_wrapper, array $settings);

}

/**
 * Class FeedItemEnhancerPlugin.
 *
 * Base class of the Feed item enhancer plugins.
 */
abstract class FeedItemEnhancerPlugin implements FeedItemEnhancerPluginPluginInterface {

  protected $feedsImporter;
  protected $feedsSource;
  protected $machineName;
  protected $settings;

  /**
   * {@inheritdoc}
   */
  public function __construct(FeedsConfigurable $feeds_configurable, $machine_name, array $settings = []) {
    if ($feeds_configurable instanceof FeedsImporter) {
      $this->feedsImporter = $feeds_configurable;
      $this->feedsSource = NULL;
    }
    elseif ($feeds_configurable instanceof FeedsSource) {
      $this->feedsImporter = feeds_importer($feeds_configurable->__get('id'));
      $this->feedsSource = $feeds_configurable;
    }
    $this->machineName = $machine_name;

    // Initialize the settings array.
    $settings += ['properties' => []];
    $this->settings = $settings;
  }

  /**
   * {@inheritdoc}
   */
  public function getSettings() {
    return $this->settings;
  }

  /**
   * Returns the properties of this enhancer plugin.
   *
   * The returned an associative array, where the keys are the machine names
   * of the properties. Each property contains an associative array with the
   * following keys and values:
   *   - label: The label of the provided property.
   *   - description: The description of the property. (optional)
   *   - property_types: The data types of the entity properties which could
   *     store the returned value of this property.
   *
   * @see https://www.drupal.org/node/905632
   *
   * @return array
   *   Array of provided properties.
   */
  protected function enhancerProperties() {
    return [];
  }

  /**
   * Return the name of the Feed Item node bundle, which attached to the
   * FeedsConfigurable.
   *
   * @return string
   *   Name of the bundle.
   */
  protected function getFeedImporterTargetNodeBundle() {
    $config = $this->feedsImporter->getConfig();
    if (!empty($config['processor']['config']['bundle'])) {
      return $config['processor']['config']['bundle'];
    }

    return FALSE;
  }

  /**
   * Returns the Feed node item's properties, grouped by their types.
   *
   * @return array
   *   An associative array, which contains the properties grouped by their
   *   types.
   */
  private function getFeedItemNodePropertiesGroupedByType() {
    $grouped_properties = [];
    if (($bundle = $this->getFeedImporterTargetNodeBundle())) {
      $grouped_properties = &drupal_static('feed_item_node_' . $bundle . '_properties_by_type', []);

      if (empty($grouped_properties)) {
        // Collect all properties of the Node bundle.
        $entity_properties = entity_get_property_info('node');
        $bundle_properties = isset($entity_properties['properties']) ? $entity_properties['properties'] : [];

        if (!empty($entity_properties['bundles'][$bundle]['properties'])) {
          $bundle_properties += $entity_properties['bundles'][$bundle]['properties'];
        }

        array_walk($bundle_properties, function ($property, $property_name) use (&$grouped_properties) {
          // Ignore properties without type.
          if (isset($property['type'])) {
            $grouped_properties[$property['type']][$property_name] = $property;
          }
        });
      }
    }

    return $grouped_properties;
  }

  /**
   * Return the properties of the Feed node item with the given type.
   *
   * @param string $property_type
   *   Type of the property. Ex.: text, boolean, list, etc.
   *
   * @return array
   *   Array of properties where the keys are the properties machine names.
   */
  protected function getFeedItemNodePropertiesWithType($property_type, array $property_conditions = []) {
    $properties_with_type = &drupal_static('feed_item_node_properties_with_' . $property_type . '_type_' . implode('_', array_values($property_conditions)), []);
    if (empty($properties_with_type)) {
      $grouped_properties = $this->getFeedItemNodePropertiesGroupedByType();

      if (empty($grouped_properties[$property_type])) {
        return [];
      }

      $properties_with_type = array_filter($grouped_properties[$property_type], function ($property) use ($property_conditions) {
        if (!empty($property_conditions)) {
          foreach ($property_conditions as $field => $value) {
            if (!empty($property[$field]) || $property[$field] != $value) {
              return FALSE;
            }
          }
        }
        return TRUE;
      });
    }

    return $properties_with_type;
  }

  /**
   * {@inheritdoc}
   */
  public function configurationForm() {
    $form = [];

    // Generate configuration form from the enhancer plugin existing
    // properties.
    $enhancer_properties = $this->enhancerProperties();

    if (!empty($enhancer_properties)) {
      $form['properties'] = [
        '#type' => 'fieldset',
        '#title' => t('Save Enhancer properties to:'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      ];

      array_walk($enhancer_properties, function ($property, $machine_name) use (&$form) {
        $attach_to_options = [];

        array_walk($property['property_types'], function ($property_type) use (&$attach_to_options) {
          $fields = $this->getFeedItemNodePropertiesWithType($property_type);
          $options = array_map(function ($field) {
            return $field['label'];
          }, $fields);
          $attach_to_options = array_merge($options, $attach_to_options);
        });

        asort($attach_to_options);

        $form['properties'][$machine_name] = [
          '#type' => 'select',
          '#title' => t('@property', ['@property' => $property['label']]),
          '#options' => $attach_to_options,
          '#empty_option' => t('- None -'),
          '#default_value' => isset($this->settings['properties'][$machine_name]) ? $this->settings['properties'][$machine_name] : '',
          '#disabled' => count($attach_to_options) == 0,
          '#description' => isset($property['description']) ? $property['description'] : '',
        ];
      });
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormValidate(array $form, array &$form_state, array &$values) {
    // Check all required form element.
    // Form elements can not be marked as required with the #required attribute,
    // because it would cause validation errors, when an enhancer plugin
    // is disabled too.
    foreach ($values as $fieldset => $field) {
      foreach ($field as $name => $value) {
        // If the field marked as required in the #states, then it should
        // not be empty.
        if (isset($form[$fieldset][$name]['#states']['required']) && empty($value)) {
          form_error($form[$fieldset][$name], t('%field field is required!', [
            '%field' => $form[$fieldset][$name]['#title'],
          ]));
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormSubmit(array $form, array &$form_state, array &$values) {
    $this->settings = $values;
    return $values;
  }

}
