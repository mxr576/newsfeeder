<?php

/**
 * Class NodeFeedItemEnhancerPlugin.
 *
 * Base class of the Node feed item enhancer plugins.
 */
abstract class NodeFeedItemEnhancerPlugin extends FeedItemEnhancerPlugin {
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
    if (!is_null($this->feedsImporter)) {
      $config = $this->feedsImporter->getConfig();
      if (!empty($config['processor']['config']['bundle'])) {
        return $config['processor']['config']['bundle'];
      }
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

}
