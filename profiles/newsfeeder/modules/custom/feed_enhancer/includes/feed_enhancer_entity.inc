<?php
/**
 * @file
 * Provides a base class and interface for FeedEnhancer entities.
 */

/**
 * Interface FeedEnhancerEntityInterface.
 */
interface FeedEnhancerEntityInterface {
  /**
   * Returns the configuration of the Enhancer entity.
   *
   * @return array
   *   Array of FeedEnhancer configuration settings.
   */
  public function getConfig();

}

/**
 * Class FeedEnhancerEntity.
 *
 * Common abstract class for FeedEnhancer entities.
 */
abstract class FeedEnhancerEntity extends Entity implements FeedEnhancerEntityInterface {
  /**
   * {@inheritdoc}
   */
  public function getConfig() {
    return $this->config;
  }

}

/**
 * Class FeedEnhancerFeedsImporterEntity.
 *
 * Base class for Feeds Importer enhancer entities.
 */
class FeedEnhancerFeedsImporterEntity extends FeedEnhancerEntity {
}

/**
 * Class FeedEnhancerFeedsSourceEntity.
 *
 * Base class for Feeds Importer enhancer entities.
 */
class FeedEnhancerFeedsSourceEntity extends FeedEnhancerEntity {
}
